#pragma once
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include "../../includes.h"
#include "../../Variables/UI.h"
#include "../il2cpp/il2cpp.h"
#include <algorithm>
#include "../../UI/Keyboard.hpp"

class noclip {
public:
	void run() {
		if (!Helper::Var::LocalPlayer || !Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer))
			return;

		if (Helper::Methods::IsInGame() || Helper::Methods::IsInLobby()) {

			if (*(bool*)(Helper::Methods::get_Data(Helper::Var::LocalPlayer) + 0x54))
				return;

			if (*(bool*)(Helper::Methods::get_Data(Helper::Var::LocalPlayer) + 0x48))
				return;

			if (UI::noclip) {
				Helper::Methods::set_layer(Helper::Methods::get_gameObject((Helper::Var::LocalPlayer)), Helper::Methods::NameToLayer(Helper::Methods::unity_string_new("Ghost")));
			}
			else {
				Helper::Methods::set_layer(Helper::Methods::get_gameObject((Helper::Var::LocalPlayer)), Helper::Methods::NameToLayer(Helper::Methods::unity_string_new("Players")));
			}
		}
	}
};