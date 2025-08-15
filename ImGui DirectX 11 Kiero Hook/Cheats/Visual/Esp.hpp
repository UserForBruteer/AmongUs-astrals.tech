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
    std::string killer, victim;
    float time;
};

static std::vector<KillFeedEntry> killFeed;
static std::unordered_set<std::string> victimsInFeed;
static std::unordered_map<std::string, bool> wasDead;

static void AddKillFeed(const std::string& killer, const std::string& victim) {
    if (!UI::event_notification || victimsInFeed.count(victim)) return;
    KillFeedEntry e{ killer.empty() ? "Unknown" : killer, victim, (float)ImGui::GetTime() };
    killFeed.push_back(e);
    victimsInFeed.insert(victim);
    NotificationSystem_class->push("<color=#FF0000>" + e.killer + " <color=#FFFFFF>killed <color=#FF0000>" + e.victim, 10, 1);
}

class esp {
public:
    rolehack* rolehack_class = new rolehack();
    void draw() {
        static float angle = 0.0f;
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
        float minDist = FLT_MAX;
        unity::vector closestPos = Helper::Var::Position_local;
        uintptr_t closestObj = Helper::Var::LocalPlayer;
        std::vector<uintptr_t> copy_NPIl = Helper::Var::NetworkedPlayerInfo_list;
        for (auto& info : copy_NPIl) {
            if (!info)
                continue;
            if (!Helper::Var::LocalPlayer)
                continue;
            if (!Helper::Methods::IsValidPtr(reinterpret_cast<void*>(info)))
                continue;
            if (!Helper::Methods::IsValidPtr(reinterpret_cast<void*>(Helper::Var::LocalPlayer)))
                continue;
            if (!Helper::Methods::IsValidPtrRegion(reinterpret_cast<void*>(info)))
                continue;
            if (!Helper::Methods::IsValidPtrRegion(reinterpret_cast<void*>(Helper::Var::LocalPlayer)))
                continue;
            if (!Helper::Methods::IsValidField(info, 0x48, sizeof(bool))) continue;
            if (*reinterpret_cast<bool*>(info + 0x48)) continue;
            if (!Helper::Methods::IsValidField(info, 0x54, sizeof(bool))) continue;
            if (*reinterpret_cast<bool*>(info + 0x48)) continue;
            if (!Helper::Methods::IsNativeObjectAlive(info))
                continue;
            auto name_sys = Helper::Methods::get_PlayerName(info);
            std::string playerName = (name_sys && name_sys->fields._stringLength > 0)
                ? utf16_to_utf8((wchar_t*)&name_sys->fields._firstChar, name_sys->fields._stringLength) : "";

            bool isDead = *reinterpret_cast<bool*>(info + 0x54);
            uintptr_t obj = *reinterpret_cast<uintptr_t*>(info + 0x58);
            if (!obj || !Helper::Methods::IsNativeObjectAlive(obj)) continue;

            bool previouslyDead = wasDead[playerName];
            if (isDead && !previouslyDead) {
                auto impostors = rolehack_class->getImpostorControl();
                uintptr_t nearestImp = 0;
                float impDist = INFINITY;
                for (auto imp : impostors) {
                    if (auto impT = Helper::Methods::get_transform(imp)) {
                        float d = unity::vector::Distance(impT->GetPosition(), Helper::Methods::get_transform(obj)->GetPosition());
                        if (d < impDist) { impDist = d; nearestImp = imp; }
                    }
                }
                std::string killer = "Unknown";
                if (nearestImp) {
                    for (auto& other : Helper::Var::NetworkedPlayerInfo_list) {
                        if (!other) continue;
                        if (*reinterpret_cast<uintptr_t*>(other + 0x58) == nearestImp) {
                            auto ksys = Helper::Methods::get_PlayerName(other);
                            if (ksys && ksys->fields._stringLength > 0)
                                killer = utf16_to_utf8((wchar_t*)&ksys->fields._firstChar, ksys->fields._stringLength);
                            break;
                        }
                    }
                }
                if (obj == Helper::Var::LocalPlayer && playerName.empty()) playerName = "You";
                AddKillFeed(killer, playerName);
            }

            if (!isDead) victimsInFeed.erase(playerName);
            wasDead[playerName] = isDead;
            if (obj == Helper::Var::LocalPlayer) continue;

            auto impostorNames = rolehack_class->getImpostorNames();
            bool isImp = std::find(impostorNames.begin(), impostorNames.end(), playerName) != impostorNames.end();

            auto transform = Helper::Methods::get_transform(obj);
            if (!transform) continue;

            auto pos = transform->GetPosition();
            float objDist = unity::vector::Distance(Helper::Var::Position_local, pos);
            if (!isDead && !isImp && objDist < minDist) {
                minDist = objDist;
                closestPos = pos;
                closestObj = obj;
            }

            auto rightTop = pos + unity::vector(transform->GetRight().x * transform->get_localScale().x / 2,
                transform->GetUp().y * transform->get_localScale().y / 2, 0);
            auto leftBottom = pos - unity::vector(transform->GetRight().x * transform->get_localScale().x / 2,
                transform->GetUp().y * transform->get_localScale().y / 2, 0);

            auto rt_screen = Helper::Methods::WorldToScreenPoint(Helper::Methods::get_current(), rightTop);
            auto lb_screen = Helper::Methods::WorldToScreenPoint(Helper::Methods::get_current(), leftBottom);
            auto start = Helper::Var::OnScreen_local;
            auto end = ImVec2(Helper::Methods::WorldToScreenPoint(Helper::Methods::get_current(), pos).x,
                screenSize.y - lb_screen.y);
            bool inVent = *reinterpret_cast<bool*>(obj + 0x48);
            ImColor col = inVent ? ImColor(0, 255, 255) : (isImp ? ImColor(255, 0, 0) : ImColor(255, 255, 255));
            ImColor bgCol = isDead ? ImColor(0, 0, 0, 50) : ImColor(0, 0, 0);
            int alpha = isDead ? 50 : 255;
            if (UI::esp && !isDead) {
                ImGui::GetBackgroundDrawList()->AddLine(start, end, ImColor((int)col.Value.x * 255, (int)col.Value.y * 255, (int)col.Value.z * 255, alpha));
            }
            if (UI::esp_box) {
                ImGui::GetBackgroundDrawList()->AddRect(ImVec2(rt_screen.x, screenSize.y - rt_screen.y),
                    ImVec2(lb_screen.x, screenSize.y - lb_screen.y),
                    bgCol, 0, 15, 3);
                ImGui::GetBackgroundDrawList()->AddRect(ImVec2(rt_screen.x, screenSize.y - rt_screen.y),
                    ImVec2(lb_screen.x, screenSize.y - lb_screen.y),
                    ImColor((int)col.Value.x * 255, (int)col.Value.y * 255, (int)col.Value.z * 255, isDead ? 15 : 255));
                if (UI::esp_dist) {
                    ImVec2 flagPos = ImVec2(rt_screen.x, screenSize.y - rt_screen.y); // чуть выше правого верхнего угла
                    char dist_str[32];
                    std::snprintf(dist_str, sizeof(dist_str), "%.1fm", objDist);
                    Helper::Methods::AddText(
                        ImGui::GetFont(),
                        ImGui::GetFontSize(),
                        true, true,
                        flagPos + ImVec2(4, 0),
                        ImColor(255, 255, 255),
                        dist_str
                    );
                }
            }
        }
        if (UI::clums) {
            // Верхний левый угол
            unity::vector topLeft_lag = unity::vector(Helper::Var::Position_lag.x - Helper::Var::size_lag.x / 2, Helper::Var::Position_lag.y - Helper::Var::size_lag.y / 2, Helper::Var::Position_lag.z);
            // Нижний правый угол
            unity::vector bottomRight_lag = unity::vector(Helper::Var::Position_lag.x + Helper::Var::size_lag.x / 2, Helper::Var::Position_lag.y + Helper::Var::size_lag.y / 2, Helper::Var::Position_lag.z);


            auto lt_lag_screen = Helper::Methods::WorldToScreenPoint(Helper::Methods::get_current(), topLeft_lag);
            auto rb_lag_screen = Helper::Methods::WorldToScreenPoint(Helper::Methods::get_current(), bottomRight_lag);

            ImGui::GetBackgroundDrawList()->AddRect(ImVec2(lt_lag_screen.x, screenSize.y - lt_lag_screen.y),
                ImVec2(rb_lag_screen.x, screenSize.y - rb_lag_screen.y),
                ImColor(0, 0, 0, 255), 0, 15, 3);
            ImGui::GetBackgroundDrawList()->AddRect(ImVec2(lt_lag_screen.x, screenSize.y - lt_lag_screen.y),
                ImVec2(rb_lag_screen.x, screenSize.y - rb_lag_screen.y),
                ImColor(0, 255, 255, 255), 0);
        }
        Helper::Var::closest = closestObj;
        if (minDist <= Helper::Var::dist_attack && UI::target_strafe) {
            float radius = Helper::Var::dist_attack - 0.1f;
            angle = fmod(angle + 2, 360.0f);
            unity::vector offset{ std::cos(angle) * radius, std::sin(angle) * radius, Helper::Var::Position_local.z };
            unity::vector around = closestPos + offset;
            Helper::Methods::get_transform(Helper::Var::LocalPlayer)->SetPosition(around);
        }
    }
};