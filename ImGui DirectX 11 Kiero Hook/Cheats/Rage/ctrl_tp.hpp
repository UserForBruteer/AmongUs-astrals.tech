#pragma once
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include "../../includes.h"
#include "../../Variables/UI.h"
#include "../il2cpp/il2cpp.h"
#include <algorithm>
#include "../../UI/Keyboard.hpp"

class ctrl_tp {
public:
    void tp() {
        if (!Helper::Var::LocalPlayer || !Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer) || !Helper::Methods::get_current() || !Helper::Methods::IsNativeObjectAlive(Helper::Methods::get_current()))
            return;
        ImVec2 ScreenSize = ImGui::GetIO().DisplaySize;

        if (KeyBoard::key_check_once(VK_CONTROL)) {
            ImVec2 mousePos = ImGui::GetMousePos();

            auto transform = Helper::Methods::get_transform(Helper::Var::LocalPlayer);
            if (!transform)
                return;

            Helper::Methods::set_position(transform, Helper::Methods::ScreenToWorldPoint(Helper::Methods::get_current(), { mousePos.x, (ScreenSize.y) - mousePos.y, 0 }));
        }
        return;
    }
};
