#pragma once
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include "../../includes.h"
#include "../../Variables/UI.h"
#include "../il2cpp/il2cpp.h"

class task_hack {
public:
	void complete() {
		if (Helper::Var::LocalPlayer) {
			if (!Helper::Methods::IsInGame() && !Helper::Methods::IsInLobby())
				return;
			uintptr_t NetPlayer = Helper::Methods::get_Data(Helper::Var::LocalPlayer);
			if (!NetPlayer)
				return;
			printf("task");
			System_Collections_Generic_List_NetworkedPlayerInfo_TaskInfo__o* Tasks = *reinterpret_cast<System_Collections_Generic_List_NetworkedPlayerInfo_TaskInfo__o**>(NetPlayer + 0x50);
			for (size_t i{}; i < Tasks->fields._size; i++) { 
				const auto& TaskInfo = Tasks->fields._items->m_Items[i];

				Helper::Methods::RpcCompleteTask(Helper::Var::LocalPlayer, (uint)TaskInfo->fields.Id);
			}
		}
	}
};