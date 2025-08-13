#pragma once

#include <WinUser.h>
#include <windows.h>
#include <string>
namespace KeyBoard {
    static std::unordered_map<int, bool> prev_key_state;
    std::string GetKeyName(int vk_code) {
        UINT scanCode = MapVirtualKeyA(vk_code, MAPVK_VK_TO_VSC);

        switch (vk_code) {
        case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
        case VK_PRIOR: case VK_NEXT:
        case VK_END: case VK_HOME:
        case VK_INSERT: case VK_DELETE:
        case VK_DIVIDE: case VK_NUMLOCK:
            scanCode |= 0x100;
            break;
        }

        char keyName[64] = { 0 };
        if (GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName))) {
            return std::string(keyName);
        }

        return "Unknown";
    }

    bool key_check_once(int key) {
        bool is_down = (GetAsyncKeyState(key) & 0x8000) != 0;
        bool was_down = prev_key_state[key];

        prev_key_state[key] = is_down;

        return is_down && !was_down;
    }

    bool key_check(int key) {
        return GetAsyncKeyState(key) != 0;
    }

    bool get_current_key(int& out_key) {
        for (int key = 8; key <= 255; ++key) {
            if (key >= VK_LBUTTON && key <= VK_RBUTTON)
                continue;

            SHORT state = GetAsyncKeyState(key);
            if ((state & 0x8000) && (state & 1)) {
                out_key = key;
                return true;
            }
        }
        return false;
    }

};