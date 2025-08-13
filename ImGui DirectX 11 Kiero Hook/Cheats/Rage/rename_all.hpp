#pragma once
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include "../../includes.h"
#include "../../Variables/UI.h"
#include "../il2cpp/il2cpp.h"
#include <iostream>

class rename_all {
public:
	void run() {
		uintptr_t GameData_c = *reinterpret_cast<uintptr_t*>(Helper::Var::gameAssembly + Helper::Var::GameData_typeinfo);
		if (!GameData_c)
			return;

		uintptr_t GameData_static = *reinterpret_cast<uintptr_t*>(GameData_c + 0x5C);
		if (!GameData_static)
			return;

		uintptr_t GameData_Instance = *reinterpret_cast<uintptr_t*>(GameData_static + 0x0);
		if (!GameData_Instance)
			return;
    	uintptr_t host = Helper::Methods::GetHost(GameData_Instance);
		uintptr_t _hostobject = *reinterpret_cast<uintptr_t*>(host + 0x58);

		if (_hostobject != Helper::Var::LocalPlayer)
			return;


		for (size_t i{}; i < Helper::Var::NetworkedPlayerInfo_list.size(); i++) {
			const auto& NetworkedPlayerInfo = Helper::Var::NetworkedPlayerInfo_list[i];
			if (!NetworkedPlayerInfo)
				continue;

			if (*(bool*)(NetworkedPlayerInfo + 0x54))
				continue;
			if (*(bool*)(NetworkedPlayerInfo + 0x48))
				continue;
			uintptr_t _object = *reinterpret_cast<uintptr_t*>(NetworkedPlayerInfo + 0x58);


			if (Helper::Methods::unity_string_new) {
				void* il2cpp_string = Helper::Methods::unity_string_new("t.me/nasral_hack");
				if (!il2cpp_string) {
					return;
				}
				Helper::Methods::CmdCheckName(_object, il2cpp_string);
			}
		}
	}
};