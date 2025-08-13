#pragma once
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include "../../includes.h"
#include "../../Variables/UI.h"
#include "../il2cpp/il2cpp.h"
#include <algorithm>
#include "../../UI/Keyboard.hpp"

class speed {
public:
	void up() {
		if (!Helper::Var::LocalPlayer || !Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer))
			return;

		uintptr_t MyPhysics = *reinterpret_cast<uintptr_t*>(Helper::Var::LocalPlayer + 0x94);
		if (!MyPhysics)
			return;

		*reinterpret_cast<float*>(MyPhysics + 0x34) = UI::speed;
	}
};