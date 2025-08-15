#pragma once
#include "../Helpers/Helper.hpp"
#include "../../kiero/minhook/include/MinHook.h"
#include <iostream>
#include "../Rage/send_ads.hpp"
#include "../Visual/RoleHack.hpp"
#include "../Rage/task_hack.hpp"
#include "../Visual/Trail.hpp"

#define HOOK_FUNC(ret, func, ...) \
ret (*old_##func)(__VA_ARGS__); \
    ret hook_##func(__VA_ARGS__)

#define HOOK_PROC(off, func_h, func_o) \
  MH_CreateHook((void*)(Helper::Var::gameAssembly + off), (void*) func_h, (void**) func_o); \
  MH_EnableHook((LPVOID)(Helper::Var::gameAssembly + off));

send_ads* send_ads_class = new send_ads();
task_hack* task_hack_class = new task_hack();

HOOK_FUNC(float, GetKillDistance, uintptr_t inst, void* method) {
    Helper::Var::dist_attack = old_GetKillDistance(inst, method);
    return Helper::Var::dist_attack;
}


HOOK_FUNC(uintptr_t, FindClosestTarget, uintptr_t inst, void* method) {
    auto orig = old_FindClosestTarget(inst, method);

    if (!orig && UI::no_cdkill && Helper::Var::LocalPlayer) {
        if (Helper::Var::LocalPlayer != Helper::Var::closest) {
            return Helper::Var::closest;
        }
    }
    return orig;
}

HOOK_FUNC(void, RpcMurderPlayer, uintptr_t inst, uintptr_t target, void* method) {
    if (inst == Helper::Var::LocalPlayer && UI::silent_kill) {
        auto transform = Helper::Methods::get_transform(Helper::Var::LocalPlayer);
        if (!transform)
            return;

        std::cout << "tp_back\n";
        Helper::Var::needback = 10;
    }
    old_RpcMurderPlayer(inst, target, method);
    return;
}

bool IsSafeDeadFlag(uintptr_t info, uintptr_t offset) noexcept {
    __try {
        volatile bool check_dead = *(bool*)(info + offset);
        (void)check_dead;
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
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
                auto position = transform->GetPosition();
                auto leftBottom = position - unity::vector(transform->GetRight().x * transform->get_localScale().x / 2,
                    transform->GetUp().y * transform->get_localScale().y / 2, 0);
                AddTrailPoint({position.x, leftBottom.y, 0}, ImColor(255, 255, 255));
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
    std::vector<uintptr_t> temp;
    temp.reserve(AllPlayers->fields._size);
    for (size_t i = 0; i < AllPlayers->fields._size; i++) {
        auto info = reinterpret_cast<uintptr_t>(AllPlayers->fields._items->m_Items[i]);
        if (!info)
            continue;

        if (!Helper::Methods::IsNativeObjectAlive(info))
            continue;

        auto obj = *(uintptr_t*)(info + 0x58);
        if (!obj || !Helper::Methods::IsNativeObjectAlive(obj))
            continue;

        auto name_sys = Helper::Methods::get_PlayerName(info);
        if (name_sys) {
            if (name_sys->fields._stringLength < 0 || name_sys->fields._stringLength > 32)
                continue;
        }

        if (!IsSafeDeadFlag(info, 0x54))
            continue;

        if (!IsSafeDeadFlag(info, 0x48))
            continue;

        uintptr_t deadFlagAddr = info + 0x54;
        uintptr_t objAddr = info + 0x58;

        if (!Helper::Methods::IsValidPtr(reinterpret_cast<void*>(deadFlagAddr), sizeof(bool)))
            continue;
        if (!Helper::Methods::IsValidPtr(reinterpret_cast<void*>(objAddr), sizeof(uintptr_t)))
            continue;


        temp.push_back(info);
    }
    Helper::Var::NetworkedPlayerInfo_list.swap(temp);

    
    return;
}

HOOK_FUNC(void, Shapeshift, uintptr_t inst, uintptr_t target, bool animate, void* method) {
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
    old_Shapeshift(inst, target, animate, method);
    return;
}

HOOK_FUNC(void, set_Visible, uintptr_t inst, bool value, void* method) {
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
    old_set_Visible(inst, value, method);
    return;
}

HOOK_FUNC(void, RpcCloseDoorsOfType, uintptr_t inst, SystemTypes value, void* method) {
    
    old_RpcCloseDoorsOfType(inst, value, method);
    if (UI::event_notification) {

        NotificationSystem_class->push(
            std::string("<color=#FF0000>") + Helper::Methods::SystemTypeToString(value) + " <color=#FFFFFF> closed" + "", 10, 1
        );
    }
}

HOOK_FUNC(float, get_ViewDistance, uintptr_t inst, void* method) {
    if (!UI::no_shadow) {
        return old_get_ViewDistance(inst, method);
    }
    return 100.f;
}

HOOK_FUNC(void, RpcSetScanner, uintptr_t inst, bool animType, void* method) {
    old_RpcSetScanner(inst, animType, method);
    printf((std::to_string(animType) + "anim\n").c_str());
    return;
}

HOOK_FUNC(void, CmdCheckMurder, uintptr_t inst, uintptr_t target, void* method) {
    if (UI::no_cdkill && inst == Helper::Var::LocalPlayer) {
        Helper::Methods::RpcMurderPlayer(inst, target, true);
        return;

    }

    return old_CmdCheckMurder(inst, target, method);
}

HOOK_FUNC(float, ShipStatus_CalculateLightRadius, uintptr_t inst, uintptr_t player, void* method) {
    if (Helper::Var::LocalPlayer) {
        if (Helper::Methods::get_Data(Helper::Var::LocalPlayer) == player && UI::no_shadow) {
            return 10000;
        }
    }

    return old_ShipStatus_CalculateLightRadius(inst, player, method);
}

HOOK_FUNC(uintptr_t, HatManager_GetUnlockedPets, uintptr_t inst, void* method) {
    if (UI::all_skins) {
        return *reinterpret_cast<uintptr_t*>(inst + 0x24);
    }
    else {
        return old_HatManager_GetUnlockedPets(inst, method);
    }
}

HOOK_FUNC(uintptr_t, HatManager_GetUnlockedHats, uintptr_t inst, void* method) {
    if (UI::all_skins) {
        return *reinterpret_cast<uintptr_t*>(inst + 0x28);
    }
    else {
        return old_HatManager_GetUnlockedHats(inst, method);
    }
}

HOOK_FUNC(uintptr_t, HatManager_GetUnlockedSkins, uintptr_t inst, void* method) {
    if (UI::all_skins) {
        return *reinterpret_cast<uintptr_t*>(inst + 0x2C);
    }
    else {
        return old_HatManager_GetUnlockedSkins(inst, method);
    }
}

HOOK_FUNC(uintptr_t, HatManager_GetUnlockedVisors, uintptr_t inst, void* method) {
    if (UI::all_skins) {
        return *reinterpret_cast<uintptr_t*>(inst + 0x30);
    }
    else {
        return old_HatManager_GetUnlockedVisors(inst, method);
    }
}

HOOK_FUNC(uintptr_t, HatManager_GetUnlockedNamePlates, uintptr_t inst, void* method) {
    if (UI::all_skins) {
        return *reinterpret_cast<uintptr_t*>(inst + 0x34);
    }
    else {
        return old_HatManager_GetUnlockedNamePlates(inst, method);
    }
}

HOOK_FUNC(System_String_o*, GameListing_get_IPString, uintptr_t inst, void* method) {
    auto string_ip = old_GameListing_get_IPString(inst, method);
    std::string c_string_ip = "localhost";
    if (string_ip && string_ip->fields._stringLength > 0)
        c_string_ip = utf16_to_utf8((wchar_t*)&string_ip->fields._firstChar, string_ip->fields._stringLength);
    
    std::cout << c_string_ip << "\n";
    Helper::Var::GetIP = true;
    Helper::Var::IP_server = c_string_ip;
    return string_ip;
}

HOOK_FUNC(void, HttpMatchmakerManagerHostServer_ctor, HttpMatchmakerManager_HostServer_o* inst, uintptr_t ip, uintptr_t port, void* method) {
    old_HttpMatchmakerManagerHostServer_ctor(inst, ip, port, method);
    
    std::string Ip_str = "localhost";
    auto Ip = inst->fields.Ip;
    if (Ip && Ip->fields._stringLength > 0)
        Ip_str = utf16_to_utf8((wchar_t*)&Ip->fields._firstChar, Ip->fields._stringLength);

    Helper::Var::GetIP = true;
    Helper::Var::IP_server = Ip_str;
    std::cout << Ip_str << std::endl;
}
int wait = 0;

HOOK_FUNC(void, CustomNetworkTransform_FixedUpdate, uintptr_t inst, void* methods) {
    if (*reinterpret_cast<uintptr_t*>(inst + 0x28) == Helper::Var::LocalPlayer && UI::clums) {
        auto trans_lag = Helper::Methods::get_transform(inst);
        if (!trans_lag->GetPointer()) {
            old_CustomNetworkTransform_FixedUpdate(inst, methods);
            return;
        }
        ImVec2 Position_lag = *(ImVec2*)(inst + 0x4C);
        Helper::Var::Position_lag = { Position_lag.x, Position_lag.y, Helper::Var::Position_local.z };
        Helper::Var::size_lag = trans_lag->get_localScale();
        if (wait <= 0) {
            wait = UI::unlag + UI::lag;
        }

        if (UI::unlag > 0 && wait > UI::lag) {
            old_CustomNetworkTransform_FixedUpdate(inst, methods);
        }
        else if (wait > 0 && wait <= UI::lag) {
        }

        if (wait > 0)
            wait--;

        return;
    }
    old_CustomNetworkTransform_FixedUpdate(inst, methods);
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
        HOOK_PROC(0x7082F0, hook_HatManager_GetUnlockedPets, &old_HatManager_GetUnlockedPets);
        HOOK_PROC(0x708270, hook_HatManager_GetUnlockedHats, &old_HatManager_GetUnlockedHats);
        HOOK_PROC(0x708330, hook_HatManager_GetUnlockedSkins, &old_HatManager_GetUnlockedSkins);
        HOOK_PROC(0x708370, hook_HatManager_GetUnlockedVisors, &old_HatManager_GetUnlockedVisors);
        HOOK_PROC(0x7082B0, hook_HatManager_GetUnlockedNamePlates, &old_HatManager_GetUnlockedNamePlates);
        HOOK_PROC(0x67C5E0, hook_GameListing_get_IPString, &old_GameListing_get_IPString);
        HOOK_PROC(0x50C540, hook_HttpMatchmakerManagerHostServer_ctor, &old_HttpMatchmakerManagerHostServer_ctor);
        HOOK_PROC(0x4C9080, hook_CustomNetworkTransform_FixedUpdate, &old_CustomNetworkTransform_FixedUpdate);
        //HOOK_PROC(0x3CABD0, hook_get_ViewDistance, &old_get_ViewDistance); 
        //HOOK_PROC(0x4CFA30, hook_ServerManager_get_TargetServer, &old_ServerManager_get_TargetServer);
    }
} 