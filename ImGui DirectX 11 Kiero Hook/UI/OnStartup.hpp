#pragma once

#include "../includes.h"
#include "../Variables/UI.h"

namespace menu {
    bool startup() {
        if (!ImGui::Begin("_komaru", nullptr))
            return false;

        ImGui::Text("Made by Komaru (@COCIJCKA).\nDefault key: Insert.");
        ImGui::Text("\nThis project is for educational purposes only - for use in private lobbies only (sure thing).");

        if (ImGui::Button("I understand")) {
            UI::startup = false;
        }

        ImGui::End();

        return true;
    }
}
