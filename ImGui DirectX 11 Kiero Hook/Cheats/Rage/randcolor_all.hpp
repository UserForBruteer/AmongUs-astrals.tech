#pragma once
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include "../../includes.h"
#include "../../Variables/UI.h"
#include "../il2cpp/il2cpp.h"
#include <iostream>

class randcolor_all {
public:
	void run() {
		srand(time(NULL));
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

		unity::vector closest = Helper::Var::Position_local;
		float dist = INFINITY;

		for (size_t i{}; i < AllPlayers->fields._size; i++) {
			const auto& NetworkedPlayerInfo = reinterpret_cast<uintptr_t>(AllPlayers->fields._items->m_Items[i]);
			if (!NetworkedPlayerInfo)
				continue;

			if (*(bool*)(NetworkedPlayerInfo + 0x54))
				continue;
			uintptr_t _object = *reinterpret_cast<uintptr_t*>(NetworkedPlayerInfo + 0x58);

			int color = rand() % 4;
			Helper::Methods::RpcSetColor(_object, NetworkedPlayerInfo);
			
		}
	}
};