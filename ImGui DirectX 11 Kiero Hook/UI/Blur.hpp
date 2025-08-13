#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <DirectXMath.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace Shaders {
    const char* VertexShaderSrc = R"(
        struct VS_OUTPUT { float4 Pos : SV_POSITION; float2 Tex : TEXCOORD; };
        VS_OUTPUT main(uint id : SV_VertexID) {
            VS_OUTPUT O;
            O.Tex = float2((id << 1) & 2, id & 2);
            O.Pos = float4(O.Tex * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
            return O;
        }
    )";

    const char* PixelShaderSrc = R"(
        cbuffer BlurSettings : register(b0) {
            float2 blurDirection;
            float blurStrength;
        };

        Texture2D txDiffuse : register(t0);
        SamplerState samLinear : register(s0);

        static const float weights[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };

        float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_Target {
            float4 color = txDiffuse.Sample(samLinear, uv) * weights[0];

            for (int i = 1; i < 5; i++) {
                color += txDiffuse.Sample(samLinear, uv + blurDirection * i * blurStrength) * weights[i];
                color += txDiffuse.Sample(samLinear, uv - blurDirection * i * blurStrength) * weights[i];
            }
            
            return color;
        }
    )";
}

struct BlurSettings {
    DirectX::XMFLOAT2 blurDirection;
    float blurStrength;
    float pad1;
};


class BlurEffect {
private:
    ID3D11VertexShader* m_pVS = nullptr;
    ID3D11PixelShader* m_pPS = nullptr;
    ID3D11SamplerState* m_pSS = nullptr;
    D3D11_VIEWPORT          m_viewport;
    ID3D11Buffer* m_pBlurSettingsCB = nullptr;
    ID3D11Texture2D* m_pTempTex = nullptr;
    ID3D11RenderTargetView* m_pTempRTV = nullptr;
    ID3D11ShaderResourceView* m_pTempSRV = nullptr;

    bool CompileShader(const char* src, const char* entry, const char* profile,
        ID3DBlob** blobOut)
    {
        ID3DBlob* errorBlob = nullptr;
        HRESULT hr = D3DCompile(src, strlen(src), nullptr, nullptr, nullptr,
            entry, profile, 0, 0, blobOut, &errorBlob);
        if (FAILED(hr)) {
            if (errorBlob) {
                std::cerr << "HLSL compile error: "
                    << (char*)errorBlob->GetBufferPointer() << std::endl;
                errorBlob->Release();
            }
            return false;
        }
        if (errorBlob) errorBlob->Release();
        return true;
    }

public:
    ~BlurEffect() { Release(); }

    void Release() {
        if (m_pVS) { m_pVS->Release(); m_pVS = nullptr; }
        if (m_pPS) { m_pPS->Release(); m_pPS = nullptr; }
        if (m_pSS) { m_pSS->Release(); m_pSS = nullptr; }
        if (m_pBlurSettingsCB) { m_pBlurSettingsCB->Release(); m_pBlurSettingsCB = nullptr; }
        if (m_pTempTex) { m_pTempTex->Release(); m_pTempTex = nullptr; }
        if (m_pTempRTV) { m_pTempRTV->Release(); m_pTempRTV = nullptr; }
        if (m_pTempSRV) { m_pTempSRV->Release(); m_pTempSRV = nullptr; }
    }

    bool Initialize(ID3D11Device* pDevice, int width, int height) {
        Release();

        ID3DBlob* vsBlob = nullptr;
        if (!CompileShader(Shaders::VertexShaderSrc, "main", "vs_5_0", &vsBlob)) return false;
        pDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_pVS);

        ID3DBlob* psBlob = nullptr;
        if (!CompileShader(Shaders::PixelShaderSrc, "main", "ps_5_0", &psBlob)) {
            vsBlob->Release(); return false;
        }
        pDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pPS);
        vsBlob->Release();
        psBlob->Release();

        D3D11_SAMPLER_DESC sd{};
        sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        pDevice->CreateSamplerState(&sd, &m_pSS);

        m_viewport = { 0.f, 0.f, (FLOAT)width, (FLOAT)height, 0.f, 1.f };

        D3D11_BUFFER_DESC cbd = {};
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.ByteWidth = sizeof(BlurSettings);
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        if (FAILED(pDevice->CreateBuffer(&cbd, nullptr, &m_pBlurSettingsCB))) {
            std::cerr << "Failed to create constant buffer" << std::endl;
            return false;
        }

        D3D11_TEXTURE2D_DESC texDesc = {};
        texDesc.Width = width;
        texDesc.Height = height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        if (FAILED(pDevice->CreateTexture2D(&texDesc, nullptr, &m_pTempTex))) {
            std::cerr << "Failed to create temporary texture" << std::endl;
            return false;
        }
        if (FAILED(pDevice->CreateRenderTargetView(m_pTempTex, nullptr, &m_pTempRTV))) {
            std::cerr << "Failed to create temporary RTV" << std::endl;
            return false;
        }
        if (FAILED(pDevice->CreateShaderResourceView(m_pTempTex, nullptr, &m_pTempSRV))) {
            std::cerr << "Failed to create temporary SRV" << std::endl;
            return false;
        }

        return true;
    }

    void Apply(ID3D11DeviceContext* pContext,
        ID3D11ShaderResourceView* srcSRV,
        ID3D11RenderTargetView* dstRTV,
        float strength = 1.0f)
    {
        if (!pContext || !srcSRV || !dstRTV || !m_pPS) return;

        pContext->VSSetShader(m_pVS, nullptr, 0);
        pContext->PSSetShader(m_pPS, nullptr, 0);
        pContext->PSSetSamplers(0, 1, &m_pSS);
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        pContext->IASetInputLayout(nullptr);
        pContext->RSSetViewports(1, &m_viewport);
        pContext->PSSetConstantBuffers(0, 1, &m_pBlurSettingsCB);

        ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        BlurSettings settings;
        settings.blurStrength = strength;

        pContext->OMSetRenderTargets(1, &m_pTempRTV, nullptr);
        pContext->PSSetShaderResources(0, 1, &srcSRV);

        pContext->Map(m_pBlurSettingsCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        settings.blurDirection = { 1.0f / m_viewport.Width, 0.0f };
        memcpy(mappedResource.pData, &settings, sizeof(BlurSettings));
        pContext->Unmap(m_pBlurSettingsCB, 0);

        pContext->Draw(3, 0);
        pContext->PSSetShaderResources(0, 1, nullSRV);

        pContext->OMSetRenderTargets(1, &dstRTV, nullptr);
        pContext->PSSetShaderResources(0, 1, &m_pTempSRV);

        pContext->Map(m_pBlurSettingsCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        settings.blurDirection = { 0.0f, 1.0f / m_viewport.Height };
        memcpy(mappedResource.pData, &settings, sizeof(BlurSettings));
        pContext->Unmap(m_pBlurSettingsCB, 0);

        pContext->Draw(3, 0);
        pContext->PSSetShaderResources(0, 1, nullSRV);
    }
};