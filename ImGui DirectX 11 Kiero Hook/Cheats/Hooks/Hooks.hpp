#pragma once
#include "../../includes.h"
#include "../../UI/Keyboard.hpp"
#include "../Helpers/Helper.hpp"
#include "../../kiero/minhook/include/MinHook.h"
#include <iostream>
#include "../Rage/send_ads.hpp"
#include "../Visual/RoleHack.hpp"
#include "../Rage/task_hack.hpp"
#define HOOK_FUNC(ret, func, ...) \
ret (*old_##func)(__VA_ARGS__); \
    ret hook_##func(__VA_ARGS__)

#define HOOK_PROC(off, func_h, func_o) \
  MH_CreateHook((void*)(Helper::Var::gameAssembly + off), (void*) func_h, (void**) func_o); \
  MH_EnableHook((LPVOID)(Helper::Var::gameAssembly + off));

send_ads* send_ads_class = new send_ads();
task_hack* task_hack_class = new task_hack();

HOOK_FUNC(float, GetKillDistance, uintptr_t inst) {
    Helper::Var::dist_attack = old_GetKillDistance(inst);
    return Helper::Var::dist_attack;
}


HOOK_FUNC(uintptr_t, FindClosestTarget, uintptr_t inst) {
    auto orig = old_FindClosestTarget(inst);

    if (!orig && UI::no_cdkill && Helper::Var::LocalPlayer) {
        if (Helper::Var::LocalPlayer != Helper::Var::closest) {
            return Helper::Var::closest;
        }
    }
    return orig;
}

HOOK_FUNC(void, RpcMurderPlayer, uintptr_t inst, uintptr_t target) {
    if (inst == Helper::Var::LocalPlayer && UI::silent_kill) {
        auto transform = Helper::Methods::get_transform(Helper::Var::LocalPlayer);
        if (!transform)
            return;

        std::cout << "tp_back\n";
        Helper::Var::needback = 12;
    }
    old_RpcMurderPlayer(inst, target);
    return;
}

HOOK_FUNC(void, InnerNetClient_Update, uintptr_t inst, void* method)  {
    if (Helper::Var::needrename) {
        Helper::Var::needrename = false;
        rename_all_class->run();
    }

    if (UI::spam) {
        static int cooldown = 0;

        if (cooldown >= 0) {
            send_ads_class->run();
            cooldown = 25 * Helper::Var::pl_size;
        }
        else {
            cooldown -= 1;
        }
    }
    if (UI::task_hack) {
       
        UI::task_hack = false;
        task_hack_class->complete();
    }
    /*if (Helper::Var::LocalPlayer && Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer)) {
        if (Helper::Methods::IsInGame()) {
            Helper::Methods::RpcSetScanner(Helper::Var::LocalPlayer, true);
        }
    }*/

    old_InnerNetClient_Update(inst, method);
    ImVec2 ScreenSize = ImGui::GetIO().DisplaySize;

    const auto& PlayerControl_TypeInfo = *reinterpret_cast<uintptr_t*>(Helper::Var::gameAssembly + Helper::Var::PlayerControl_TypeInfo);
    if (PlayerControl_TypeInfo) {
        const auto& PlayerControl_StaticField = *reinterpret_cast<uintptr_t*>(PlayerControl_TypeInfo + 0x5C);
        if (PlayerControl_StaticField) {
            const auto& strct = reinterpret_cast<PlayerControl_StaticFields*>(PlayerControl_StaticField);
            if (strct) {
                if (!reinterpret_cast<uintptr_t>(strct->LocalPlayer))
                    return;

                Helper::Var::LocalPlayer = reinterpret_cast<uintptr_t>(strct->LocalPlayer);

                if (!Helper::Var::LocalPlayer || !Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer) || !Helper::Methods::get_current() || !Helper::Methods::IsNativeObjectAlive(Helper::Methods::get_current()))
                    return;

                auto transform = Helper::Methods::get_transform(Helper::Var::LocalPlayer);
                if (!transform)
                    return;
                if (Helper::Var::needback != 0 && UI::silent_kill) {
                    Helper::Var::needback -= 1;
                    Helper::Methods::set_position(transform, Helper::Var::Position_local);
                }
                auto position = Helper::Methods::get_position(transform);
                unity::vector onscreen = Helper::Methods::WorldToScreenPoint(Helper::Methods::get_current(), position);
                Helper::Var::OnScreen_local = { onscreen.x, ScreenSize.y - onscreen.y };
                Helper::Var::Position_local = position;

            }
        }
    }

    uintptr_t GameData_c = *reinterpret_cast<uintptr_t*>(Helper::Var::gameAssembly + Helper::Var::GameData_typeinfo);
    if (!GameData_c)
        return;

    uintptr_t GameData_static = *reinterpret_cast<uintptr_t*>(GameData_c + 0x5C);
    if (!GameData_static)
        return;
    uintptr_t GameData_Instance = *reinterpret_cast<uintptr_t*>(GameData_static + 0x0);
    if (!GameData_Instance)
        return;
    const auto& AllPlayers = *reinterpret_cast<System_Collections_Generic_List_NetworkedPlayerInfo__o**>(GameData_Instance + 0x10);
    if (!AllPlayers)
        return;

    Helper::Var::pl_size = AllPlayers->fields._size;
    UI::count_pl = AllPlayers->fields._size;
    Helper::Var::NetworkedPlayerInfo_list.clear();
    for (size_t i{}; i < AllPlayers->fields._size; i++) {
        const auto& NetworkedPlayerInfo = reinterpret_cast<uintptr_t>(AllPlayers->fields._items->m_Items[i]);
        if (!NetworkedPlayerInfo)
            continue;

        Helper::Var::NetworkedPlayerInfo_list.push_back(NetworkedPlayerInfo);
    }
    
    return;
}

HOOK_FUNC(void, Shapeshift, uintptr_t inst, uintptr_t target, bool animate) {
    if (UI::event_notification) {
        if (inst != target) {
            uintptr_t inst_network = Helper::Methods::get_Data(inst);
            System_String_o* isys = Helper::Methods::get_PlayerName(inst_network);
            std::string morph_name;
            if (isys && isys->fields._stringLength > 0) {
                morph_name = utf16_to_utf8(
                    reinterpret_cast<const wchar_t*>(&isys->fields._firstChar),
                    isys->fields._stringLength
                );
            }

            uintptr_t target_network = Helper::Methods::get_Data(target);
            System_String_o* tsys = Helper::Methods::get_PlayerName(target_network);
            std::string morphed_name;
            if (tsys && tsys->fields._stringLength > 0) {
                morphed_name = utf16_to_utf8(
                    reinterpret_cast<const wchar_t*>(&tsys->fields._firstChar),
                    tsys->fields._stringLength
                );
            }
            NotificationSystem_class->push(
                std::string("<color=#FF0000>") + morph_name + " <color=#FFFFFF>morphed to <color=#FF0000>" + morphed_name + "", 10, 1
            );
        }
        else {
            uintptr_t inst_network = Helper::Methods::get_Data(inst);
            System_String_o* isys = Helper::Methods::get_PlayerName(inst_network);
            std::string morph_name;
            if (isys && isys->fields._stringLength > 0) {
                morph_name = utf16_to_utf8(
                    reinterpret_cast<const wchar_t*>(&isys->fields._firstChar),
                    isys->fields._stringLength
                );
            }

            NotificationSystem_class->push(
                std::string("<color=#FF0000>") + morph_name + " <color=#FFFFFF>morphed back" + "", 10, 1
            );
        }
    }
    old_Shapeshift(inst, target, animate);
    return;
}

HOOK_FUNC(void, set_Visible, uintptr_t inst, bool value) {
    if (UI::event_notification) {
        if (value) {
            uintptr_t inst_network = Helper::Methods::get_Data(inst);
            System_String_o* isys = Helper::Methods::get_PlayerName(inst_network);
            std::string morph_name;
            if (isys && isys->fields._stringLength > 0) {
                morph_name = utf16_to_utf8(
                    reinterpret_cast<const wchar_t*>(&isys->fields._firstChar),
                    isys->fields._stringLength
                );
            }

            NotificationSystem_class->push(
                std::string("<color=#FF0000>") + morph_name + " <color=#FFFFFF> in invisible" + "", 10, 1
            );
        }
        else {
            uintptr_t inst_network = Helper::Methods::get_Data(inst);
            System_String_o* isys = Helper::Methods::get_PlayerName(inst_network);
            std::string morph_name;
            if (isys && isys->fields._stringLength > 0) {
                morph_name = utf16_to_utf8(
                    reinterpret_cast<const wchar_t*>(&isys->fields._firstChar),
                    isys->fields._stringLength
                );
            }

            NotificationSystem_class->push(
                std::string("<color=#FF0000>") + morph_name + " <color=#FFFFFF> out invisible" + "", 10, 1
            );
        }
    }
    old_set_Visible(inst, value);
    return;
}

HOOK_FUNC(void, RpcCloseDoorsOfType, uintptr_t inst, SystemTypes value) {
    
    old_RpcCloseDoorsOfType(inst, value);
    if (UI::event_notification) {

        NotificationSystem_class->push(
            std::string("<color=#FF0000>") + Helper::Methods::SystemTypeToString(value) + " <color=#FFFFFF> closed" + "", 10, 1
        );
    }
}

HOOK_FUNC(float, get_ViewDistance, uintptr_t inst) {
    if (!UI::no_shadow) {
        return old_get_ViewDistance(inst);
    }
    return 100.f;
}

HOOK_FUNC(void, RpcSetScanner, uintptr_t inst, bool animType) {
    old_RpcSetScanner(inst, animType);
    printf((std::to_string(animType) + "anim\n").c_str());
    return;
}

HOOK_FUNC(void, CmdCheckMurder, uintptr_t inst, uintptr_t target) {
    if (UI::no_cdkill && inst == Helper::Var::LocalPlayer) {
        Helper::Methods::RpcMurderPlayer(inst, target, true);
        return;

    }

    return old_CmdCheckMurder(inst, target);
}

HOOK_FUNC(float , ShipStatus_CalculateLightRadius, uintptr_t inst, uintptr_t player) {
    if (Helper::Var::LocalPlayer) {
        if (Helper::Methods::get_Data(Helper::Var::LocalPlayer) == player && UI::no_shadow) {
            return 10000;
        }
    }

    return old_ShipStatus_CalculateLightRadius(inst, player);
}
namespace hooks {
    
    void Init() {
        HOOK_PROC(0x7575C0, hook_GetKillDistance, &old_GetKillDistance);
        HOOK_PROC(0x59DC90, hook_FindClosestTarget, &old_FindClosestTarget);
        HOOK_PROC(0x55C580, hook_RpcMurderPlayer, &old_RpcMurderPlayer);
        HOOK_PROC(0x685300, hook_InnerNetClient_Update, &old_InnerNetClient_Update);
        HOOK_PROC(0x55F600, hook_Shapeshift, &old_Shapeshift);
        HOOK_PROC(0x55E4D0, hook_set_Visible, &old_set_Visible);
        HOOK_PROC(0x5C7380, hook_RpcCloseDoorsOfType, &old_RpcCloseDoorsOfType);
        HOOK_PROC(0x55D330, hook_RpcSetScanner, &old_RpcSetScanner);
        //HOOK_PROC(0x5557E0, hook_CmdCheckMurder, &old_CmdCheckMurder);
        HOOK_PROC(0x5C5900, hook_ShipStatus_CalculateLightRadius, &old_ShipStatus_CalculateLightRadius);
        //HOOK_PROC(0x3CABD0, hook_get_ViewDistance, &old_get_ViewDistance);
    }
} 