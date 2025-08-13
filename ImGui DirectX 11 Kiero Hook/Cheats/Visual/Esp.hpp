#pragma once
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include "../../includes.h"
#include "../../Variables/UI.h"
#include "../il2cpp/il2cpp.h"
#include "Cheats/Visual/RoleHack.hpp"
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <cmath>

struct KillFeedEntry {
    std::string killer;
    std::string victim;
    float time;
};

static std::vector<KillFeedEntry> killFeed;
static std::unordered_set<std::string> victimsInFeed;
static std::unordered_map<std::string, bool> wasDead;

static void AddKillFeed(const std::string& killerName, const std::string& victimName) {
    if (UI::event_notification) {
        if (victimsInFeed.count(victimName))
            return;

        KillFeedEntry e;
        e.killer = killerName.empty() ? std::string("Unknown") : killerName;
        e.victim = victimName;
        e.time = (float)ImGui::GetTime();

        killFeed.push_back(e);
        victimsInFeed.insert(victimName);
        NotificationSystem_class->push(
            std::string("<color=#FF0000>") + e.killer + " <color=#FFFFFF>killed <color=#FF0000>" + e.victim + "", 10, 1
        );
    }
}

class esp {
public:
    rolehack* rolehack_class = new rolehack();
    void draw() {
        static float angle = 0.0f;
        ImVec2 ScreenSize = ImGui::GetIO().DisplaySize;
        float dist = FLT_MAX;
        unity::vector closest = Helper::Var::Position_local;
        uintptr_t var_closest = Helper::Var::LocalPlayer;
        for (size_t i{}; i < Helper::Var::NetworkedPlayerInfo_list.size(); i++) {
            const auto& NetworkedPlayerInfo = Helper::Var::NetworkedPlayerInfo_list[i];
            if (!NetworkedPlayerInfo)
                continue;

            if (!Helper::Var::LocalPlayer || !Helper::Methods::IsNativeObjectAlive(NetworkedPlayerInfo))
                continue;

            if (*(bool*)(NetworkedPlayerInfo + 0x48))
                continue;

            System_String_o* name_sys = Helper::Methods::get_PlayerName(NetworkedPlayerInfo);
            std::string playerName;
            if (name_sys && name_sys->fields._stringLength > 0) {
                playerName = utf16_to_utf8(
                    reinterpret_cast<const wchar_t*>(&name_sys->fields._firstChar),
                    name_sys->fields._stringLength
                );
            }

            bool isDead = *(bool*)(NetworkedPlayerInfo + 0x54);

            uintptr_t _object = *reinterpret_cast<uintptr_t*>(NetworkedPlayerInfo + 0x58);
            if (!_object || !Helper::Methods::IsNativeObjectAlive(_object))
                continue;

            if (_object == Helper::Var::LocalPlayer) {
                if (UI::imp) {
                    *reinterpret_cast<RoleTypes*>(NetworkedPlayerInfo + 0x38) = RoleTypes::Engineer;
                    auto kal = *reinterpret_cast<uintptr_t*>(NetworkedPlayerInfo + 0x4C);
                    if (kal) {
                        *reinterpret_cast<bool*>(kal + 0x43) = true;
                        Helper::Methods::RpcSetRole(_object, RoleTypes::Engineer, true);
                    }
                }
            }

            bool previouslyDead = false;
            auto itW = wasDead.find(playerName);
            if (itW != wasDead.end()) previouslyDead = itW->second;

            if (isDead && !previouslyDead) {
                auto impostors = rolehack_class->getImpostorControl();
                uintptr_t nearestImp = 0;
                float minDist = INFINITY;
                for (auto imp : impostors) {
                    auto impTrans = Helper::Methods::get_transform(imp);
                    if (!impTrans) continue;
                    float d = unity::vector::Distance(
                        Helper::Methods::get_position(impTrans),
                        Helper::Methods::get_position(Helper::Methods::get_transform(_object))
                    );
                    if (d < minDist) {
                        minDist = d;
                        nearestImp = imp;
                    }
                }
                std::string killerName = "Unknown";
                if (nearestImp) {
                    for (size_t j = 0; j < Helper::Var::NetworkedPlayerInfo_list.size(); ++j) {
                        const auto& otherInfo = Helper::Var::NetworkedPlayerInfo_list[j];
                        if (!otherInfo) continue;
                        uintptr_t otherObj = *reinterpret_cast<uintptr_t*>(otherInfo + 0x58);
                        if (otherObj == nearestImp) {
                            System_String_o* ksys = Helper::Methods::get_PlayerName(otherInfo);
                            if (ksys && ksys->fields._stringLength > 0) {
                                killerName = utf16_to_utf8(
                                    reinterpret_cast<const wchar_t*>(&ksys->fields._firstChar),
                                    ksys->fields._stringLength
                                );
                            }
                            break;
                        }
                    }
                }

                if (_object == Helper::Var::LocalPlayer && playerName.empty()) {
                    playerName = std::string("You");
                }

                AddKillFeed(killerName, playerName);
            }
            if (!isDead) {
                if (victimsInFeed.count(playerName)) {
                    victimsInFeed.erase(playerName);
                }
            }
            wasDead[playerName] = isDead;
            if (Helper::Var::LocalPlayer == _object)
                continue;
            std::vector<std::string> imp = rolehack_class->getImpostorNames();
            auto it = std::find(imp.begin(), imp.end(), playerName);

            auto transform = Helper::Methods::get_transform(_object);
            if (!transform)
                continue;

            
            auto position = Helper::Methods::get_position(transform);
            float _objdist = unity::vector::Distance(Helper::Var::Position_local, position);
            if (!*(bool*)(NetworkedPlayerInfo + 0x54)) {
                

                if (it == imp.end()) {
                    if (dist > _objdist) {
                        dist = _objdist;
                        closest = position;
                        var_closest = _object;
                    }
                }
            }
            unity::vector onscreen = Helper::Methods::WorldToScreenPoint(Helper::Methods::get_current(), position);
            ImVec2 start = Helper::Var::OnScreen_local;
            ImVec2 end = { onscreen.x, ScreenSize.y - onscreen.y };

            if (it != imp.end()) {
                if (*(bool*)(NetworkedPlayerInfo + 0x54)) {
                    ImGui::GetBackgroundDrawList()->AddLine(start, end, ImColor(255, 0, 0, 122));
                }
                else {
                    ImGui::GetBackgroundDrawList()->AddLine(start, end, ImColor(255, 0, 0, 255));
                }
            }
            else {
                if (*(bool*)(NetworkedPlayerInfo + 0x54)) {
                    ImGui::GetBackgroundDrawList()->AddLine(start, end, ImColor(255, 255, 255, 122));
                }
                else {
                    ImGui::GetBackgroundDrawList()->AddLine(start, end, ImColor(255, 255, 255, 255));
                }
            }

            ImVec2 center = { (start.x + end.x) * 0.5f, (start.y + end.y) * 0.5f };

            float dx = end.x - start.x;
            float dy = end.y - start.y;
            float angle = std::atan2(dy, dx);

            char dist_str[32];
            std::snprintf(dist_str, sizeof(dist_str), "%.2fm", _objdist);

            Helper::Methods::AddText(ImGui::GetFont(), ImGui::GetFontSize(), true, true, center, ImColor(255, 255, 255), dist_str);
        }

        Helper::Var::closest = var_closest;
        if (dist <= Helper::Var::dist_attack && UI::target_strafe) {
            float radius = Helper::Var::dist_attack - 0.1f;
            
            unity::vector offset;
            angle += 2;
            if (angle == 360)
                angle = 0;
            offset.x = std::cos(angle) * radius;
            offset.y = std::sin(angle) * radius;
            offset.z = Helper::Var::Position_local.z;

            unity::vector around = closest + offset;
            unity::vector direction_to_destination = around - Helper::Var::Position_local;
            float distance_to_destination = direction_to_destination.magnitude();
            auto norm = unity::vector::Normalize(closest - around);
            bool target_is_visible = Helper::Methods::Raycast({ closest.x, closest.y }, {norm.x, norm.y}, unity::vector::Distance(around, closest), Helper::Methods::NameToLayer(Helper::Methods::unity_string_new("Players"))).m_Collider == 0;

            if (target_is_visible) {
                Helper::Methods::set_position(Helper::Methods::get_transform(Helper::Var::LocalPlayer), around);
            }
        }
    };
};
