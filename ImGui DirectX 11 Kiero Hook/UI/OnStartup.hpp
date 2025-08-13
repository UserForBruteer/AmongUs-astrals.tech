#pragma once

#include "../includes.h"
#include "../Variables/UI.h"

namespace menu {
    bool startup() {
        if (!ImGui::Begin("_komaru", nullptr))
            return false;

        ImGui::Text("Made by Komaru (@COCIJCKA)\nDefault key: Insert");

        if (ImGui::Button("I understand")) {
            UI::startup = false;
        }

        ImGui::End();

        return true;
    }
}
