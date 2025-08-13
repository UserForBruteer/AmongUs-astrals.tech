#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "includes.h"
#include "UI/UI.hpp"
#include "Cheats/Helpers/Helper.hpp"
#include "UI/OnStartup.hpp"
#include "Cheats/Hooks/Hooks.hpp"
#include "Cheats/Visual/Esp.hpp"
#include <iostream>
#include "Cheats/Rage/task_hack.hpp"
#include "UI/Blur.hpp"
#include "Cheats/Visual/RoleHack.hpp"
#include "UI/Font.h"
#include "Cheats/Rage/ctrl_tp.hpp"
#include "Cheats/Rage/speed.hpp"
#include "Cheats/Rage/vanish.hpp"
#include "Cheats/Rage/noclip.hpp"
#include <d3d11.h>
#include <D3DX11core.h>
#include <D3DX11tex.h>
#include <d3dcompiler.h>
#include "Variables/Astrals.h"
#include "Variables/mihail2.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3dx11.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); 
// uwu nya~ :ç, by komaru(@COCIJCKA)
Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

bool LoadImageFromMemory(ID3D11Device* device, unsigned char* image, size_t image_size, ID3D11ShaderResourceView** result) {
	D3DX11_IMAGE_LOAD_INFO information;
	ID3DX11ThreadPump* thread = nullptr;

	auto hres = D3DX11CreateShaderResourceViewFromMemory(device, image, image_size, &information, thread, result, 0);
	return (hres == S_OK);
}


void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

	ImFontConfig config;
	config.OversampleH = 2;
	config.OversampleV = 2;
	config.PixelSnapH = true;
	io.Fonts->AddFontFromMemoryTTF(&Fonts::Nunito, sizeof(Fonts::Nunito), 16.f, &config, io.Fonts->GetGlyphRangesCyrillic());
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}
esp* esp_class = new esp();
rolehack* rolehack_class = new rolehack();
ctrl_tp* ctrl_tp_class = new ctrl_tp();
speed* speed_class = new speed();
vanish* vanish_class = new vanish();
noclip* noclip_class = new noclip();

bool init = false;
BlurEffect g_blurEffect;
ID3D11Texture2D* g_pResolvedTexture = nullptr;
ID3D11ShaderResourceView* g_pResolvedSRV = nullptr;
ID3D11Texture2D* g_pBlurredTexture = nullptr;
ID3D11RenderTargetView* g_pBlurredRTV = nullptr;
ID3D11ShaderResourceView* g_pBlurredSRV = nullptr;
bool g_needsScreenshot = true;
float blur = 0.f;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;

			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);

			D3D11_TEXTURE2D_DESC desc;
			pBackBuffer->GetDesc(&desc);
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			D3D11_BLEND_DESC blendDesc = {};
			blendDesc.AlphaToCoverageEnable = FALSE;
			blendDesc.IndependentBlendEnable = FALSE;
			blendDesc.RenderTarget[0].BlendEnable = TRUE;
			blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			ID3D11BlendState* blendState;
			pDevice->CreateBlendState(&blendDesc, &blendState);
			float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
			pContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

			pDevice->CreateTexture2D(&desc, nullptr, &g_pResolvedTexture);
			if (g_pResolvedTexture) {
				pDevice->CreateShaderResourceView(g_pResolvedTexture, nullptr, &g_pResolvedSRV);
			}

			g_blurEffect.Initialize(pDevice, desc.Width, desc.Height);
			pBackBuffer->Release();

			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			bool res = LoadImageFromMemory(pDevice, raw_logo::kmr_hk, sizeof(raw_logo::kmr_hk), &UI::logo);
			if (!res) {
				printf("Uh oh! Wat wrong w/ res!\n");
				return oPresent(pSwapChain, SyncInterval, Flags);
			}
			bool res2 = LoadImageFromMemory(pDevice, Mihail2::rawData, sizeof(Mihail2::rawData), &UI::misha);
			if (!res) {
				printf("Uh oh! Wat wrong w/ res!\n");
				return oPresent(pSwapChain, SyncInterval, Flags);
			}
			init = true;
			
		}
		else { return oPresent(pSwapChain, SyncInterval, Flags); }
	}

	if (!UI::open)
	{
		if (blur < 2) {
			blur += 0.1;
		}
	}
	else {
		if (blur > 0.f) {
			blur -= 0.1;
		}
	}
	if (blur > 0.f) {
		ID3D11Texture2D* pBackBuffer = nullptr;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
		if (pBackBuffer && g_pResolvedTexture && g_pResolvedSRV) {
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			pContext->ResolveSubresource(g_pResolvedTexture, 0, pBackBuffer, 0, sd.BufferDesc.Format);

			g_blurEffect.Apply(pContext, g_pResolvedSRV, mainRenderTargetView, blur);
		}
		if (pBackBuffer) pBackBuffer->Release();

		HRESULT hr = pDevice->GetDeviceRemovedReason();
		if (FAILED(hr)) {
			std::cout << "DEVICE REMOVED, HRESULT: " << hr << std::endl;
		}
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(5, 5), ImVec2(35, 35), ImColor(49, 71, 71, 122), 5);
	ImGui::GetForegroundDrawList()->AddRect(ImVec2(5, 5), ImVec2(35, 35), ImColor(49, 71, 71, 255), 5, 15, 2);
	ImGui::GetForegroundDrawList()->AddImage((void*)UI::logo, ImVec2(10, 10), ImVec2(30, 30));

	if (!UI::_MISHA) {
		if (UI::startup) {
			menu::startup();
		}
		else {
			menu2::MenuRenderer::Get().Render();
			if (UI::esp)
				esp_class->draw();
			if (UI::rolehack)
			{
				rolehack_class->draw();
			}
			if (UI::ctrl_tp) {
				ctrl_tp_class->tp();
			}
			if (UI::speed_check) {
				speed_class->up();
			}
			vanish_class->jump();
			noclip_class->run();
		}
	}
	else {
		auto io = ImGui::GetIO();
		ImGui::GetForegroundDrawList()->AddImage((void*)UI::misha, ImVec2(0, 0), io.DisplaySize);
	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pSwapChain, SyncInterval, Flags);
}
void open_console() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	freopen("CONIN$", "r", stdin);

	SetConsoleTitleA("@nasral_hack");

	std::cout << "[+] Console initialized!" << std::endl;
}
DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			Helper::Methods::init();
			#ifdef _DEBUG
			open_console();
			#else
			// nothing todo
			#endif
			hooks::Init();
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}