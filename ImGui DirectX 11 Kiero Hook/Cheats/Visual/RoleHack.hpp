#pragma once

#include "../../includes.h"
#include "../il2cpp/il2cpp.h"
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <codecvt>
#include <locale>
#include <Windows.h>
#include <string>

std::string utf16_to_utf8(const wchar_t* wstr, int len) {
    if (!wstr || len == 0) return "";

    int utf8_len = WideCharToMultiByte(CP_UTF8, 0, wstr, len, nullptr, 0, nullptr, nullptr);
    if (utf8_len == 0) return "";

    std::string utf8str(utf8_len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr, len, &utf8str[0], utf8_len, nullptr, nullptr);

    return utf8str;
}

class rolehack {
private:
    ImVec2 listPos = ImVec2(200.0f, 200.0f);
    float smoothFactor = 0.03f;

public:
    std::vector<std::string> getImpostorNames() {
        std::vector<std::string> impostorNames;
        if (!Helper::Var::LocalPlayer || !Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer))
            return impostorNames;

        if (!Helper::Methods::IsInGame() && !Helper::Methods::IsInLobby())
            return impostorNames;

        for (int i = 0; i < Helper::Var::NetworkedPlayerInfo_list.size(); i++) {
            uintptr_t playerInfo = Helper::Var::NetworkedPlayerInfo_list[i];
            if (playerInfo || Helper::Methods::IsNativeObjectAlive(playerInfo)) {
                if (*(bool*)(playerInfo + 0x54))
                    continue;

                if (*(bool*)(playerInfo + 0x48))
                    continue;

                RoleTypes role = *reinterpret_cast<RoleTypes*>(playerInfo + 0x38);

                if (role == RoleTypes::Impostor || role == RoleTypes::Shapeshifter || role == RoleTypes::Phantom) {
                    System_String_o* playerName_System = Helper::Methods::get_PlayerName(playerInfo);
                    if (playerName_System && playerName_System->fields._stringLength > 0) {
                        std::string name = utf16_to_utf8(
                            reinterpret_cast<const wchar_t*>(&playerName_System->fields._firstChar),
                            playerName_System->fields._stringLength
                        );
                        impostorNames.push_back(name);
                    }
                }
            }
        }

        return impostorNames;
    };

    std::vector<uintptr_t> getImpostorControl() {
        std::vector<uintptr_t> impostorNames;
        if (!Helper::Var::LocalPlayer || !Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer))
            return impostorNames;

        if (!Helper::Methods::IsInGame() && !Helper::Methods::IsInLobby())
            return impostorNames;

        for (int i = 0; i < Helper::Var::NetworkedPlayerInfo_list.size(); i++) {
            uintptr_t playerInfo = Helper::Var::NetworkedPlayerInfo_list[i];
            if (playerInfo || Helper::Methods::IsNativeObjectAlive(playerInfo)) {
                if (*(bool*)(playerInfo + 0x54))
                    continue;

                if (*(bool*)(playerInfo + 0x48))
                    continue;

                uintptr_t _object = *reinterpret_cast<uintptr_t*>(playerInfo + 0x58);

                if (!_object || !Helper::Methods::IsNativeObjectAlive(_object))
                    continue;

                RoleTypes role = *reinterpret_cast<RoleTypes*>(playerInfo + 0x38);

                if (role == RoleTypes::Impostor || role == RoleTypes::Shapeshifter || role == RoleTypes::Phantom) {
                    impostorNames.push_back(_object);
                }
            }
        }

        return impostorNames;
    };

    void draw() {
        if (!Helper::Var::LocalPlayer || !Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer))
            return;
        std::vector<std::string> impostorNames = getImpostorNames();
        if (impostorNames.empty()) {
            return;
        }

        ImDrawList* drawList = ImGui::GetBackgroundDrawList();

        const float fontSize = 24.0f;
        const float lineSpacing = 25.0f;
        const ImVec2 padding = ImVec2(10.0f, 10.0f);

        float maxTextWidth = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, "Impostors:").x;
        for (const auto& name : impostorNames) {
            float textWidth = ImGui::GetFont()->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, name.c_str()).x;
            if (textWidth > maxTextWidth) {
                maxTextWidth = textWidth;
            }
        }

        ImVec2 listSize = ImVec2(
            maxTextWidth + padding.x * 2,
            (impostorNames.size() + 1) * lineSpacing + padding.y * 2 - (lineSpacing - fontSize)
        );

        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
        ImVec2 playerPos = Helper::Var::OnScreen_local;
        ImVec2 targetPos;

        bool isPlayerOnScreen = (playerPos.x > 1 && playerPos.x < screenSize.x - 1 &&
            playerPos.y > 1 && playerPos.y < screenSize.y - 1);

        if (isPlayerOnScreen) {
            float verticalOffset = 120.0f;
            targetPos = ImVec2(
                playerPos.x - listSize.x / 2,
                playerPos.y - listSize.y - verticalOffset
            );
        }
        else {
            targetPos = ImVec2(25.0f, 15.0f);
        }

        listPos.x += (targetPos.x - listPos.x) * smoothFactor;
        listPos.y += (targetPos.y - listPos.y) * smoothFactor;

        auto drawListContent = [&](ImVec2 pos) {
            ImVec2 listEndPos = ImVec2(pos.x + listSize.x, pos.y + listSize.y);
            ImU32 textColor = IM_COL32(255, 70, 70, 255);
            ImVec2 currentTextPos = ImVec2(pos.x + padding.x, pos.y + padding.y);

            drawList->AddRectFilled(pos, listEndPos, IM_COL32(30, 30, 30, 200), 8.0f);
            drawList->AddRect(pos, listEndPos, IM_COL32(200, 50, 50, 220), 8.0f, 0, 1.0);

            drawList->AddText(ImGui::GetFont(), fontSize, currentTextPos, textColor, "Impostors:");
            currentTextPos.y += lineSpacing;
            for (const auto& name : impostorNames) {
                drawList->AddText(ImGui::GetFont(), fontSize, currentTextPos, textColor, name.c_str());
                currentTextPos.y += lineSpacing;
            }
            };

        drawListContent(listPos);

        if (isPlayerOnScreen) {
            ImVec2 listBottomCenter = ImVec2(listPos.x + listSize.x / 2, listPos.y + listSize.y);

            drawList->AddCircleFilled(playerPos, 5.0f, IM_COL32(255, 255, 255, 200), 12);

            drawList->AddLine(playerPos, listBottomCenter, IM_COL32(255, 255, 255, 150), 2.0f);
        }
    }
};