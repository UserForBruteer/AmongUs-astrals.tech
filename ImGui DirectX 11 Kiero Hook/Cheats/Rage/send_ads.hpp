#pragma once
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include "../../includes.h"
#include "../../Variables/UI.h"
#include "../il2cpp/il2cpp.h"
#include <iostream>

class send_ads {
public:
	void run() {
		if (!Helper::Var::LocalPlayer)
			return;
		unity::vector closest = Helper::Var::Position_local;
		float dist = INFINITY;

		uintptr_t _object = Helper::Var::LocalPlayer;

		if (Helper::Methods::unity_string_new) {
			void* il2cpp_string = Helper::Methods::unity_string_new("telegram nasral_hack");
			if (!il2cpp_string) {
				return;
			}
			Helper::Methods::RpcSendChat(_object, il2cpp_string);
		}
		
	}
};