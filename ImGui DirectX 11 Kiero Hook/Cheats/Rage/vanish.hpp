#pragma once
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include "../../includes.h"
#include "../../Variables/UI.h"
#include "../il2cpp/il2cpp.h"

class vanish {
public:
	void jump() {
		if (!Helper::Var::LocalPlayer || !Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer))
			return;
		if (UI::vanish) {
			if (!Helper::Var::wassss) {
				Helper::Var::wassss = true;
				Helper::Methods::RpcEnterVent(*reinterpret_cast<uintptr_t*>(Helper::Var::LocalPlayer + 0x94), 1);
				*reinterpret_cast<bool*>(Helper::Var::LocalPlayer + 0x48) = false;
				*reinterpret_cast<bool*>(Helper::Var::LocalPlayer + 0x49) = false;
			}
			else {
				*reinterpret_cast<bool*>(Helper::Var::LocalPlayer + 0x49) = false;
				*reinterpret_cast<bool*>(Helper::Var::LocalPlayer + 0x48) = false;
			}
		}
		else {
			if (Helper::Var::wassss) {
				Helper::Methods::RpcExitVent(*reinterpret_cast<uintptr_t*>(Helper::Var::LocalPlayer + 0x94));
				Helper::Var::wassss = false;
			}
		}

	}
};