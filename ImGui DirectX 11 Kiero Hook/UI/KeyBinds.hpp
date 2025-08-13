#pragma once

#include "UI/Keyboard.hpp"
#include "Variables/UI.h"
#include <map>
#include <set>
#include <memory>
#include <type_traits>
#include <iterator>

namespace keybinds {
    void assign_keybind(int key, bool* toggle_ptr, BindMode mode) {
        if (key == VK_ESCAPE || !toggle_ptr) return;

        for (auto it = UI::key_binds.begin(); it != UI::key_binds.end(); ) {
            auto& binds = it->second;
            binds.erase(std::remove_if(binds.begin(), binds.end(), [&](const BindInfo& info) {
                return info.target == toggle_ptr;
                }), binds.end());

            if (binds.empty()) {
                it = UI::key_binds.erase(it);
            }
            else {
                ++it;
            }
        }


        UI::key_binds[key].push_back({ toggle_ptr, mode, false, UI::current_bind_name });
    }


    void process_keybinds() {
        for (auto& [key, binds] : UI::key_binds) {
            if (key == UI::bind_open) continue;

            bool is_pressed = KeyBoard::key_check(key);
            bool is_clicked = KeyBoard::key_check_once(key);

            for (auto& bind : binds) {
                if (bind.mode == BindMode::Toggle) {
                    if (is_clicked) {
                        *bind.target = !(*bind.target);
                        if (UI::event_notification) {
                            if (*bind.target) {
                                NotificationSystem_class->push(
                                    std::string("Turn <color=#00FF00>ON <color=#FFFFFF>") + bind.name + "", 10, 1
                                );
                            }
                            else {
                                NotificationSystem_class->push(
                                    std::string("Turn <color=#FF0000>OFF <color=#FFFFFF>") + bind.name + "", 10, 1
                                );
                            }
                        }
                    }
                }
                else if (bind.mode == BindMode::Hold) {
                    if (is_pressed && !bind.last_hold_state)
                        *bind.target = true;
                    else if (!is_pressed && bind.last_hold_state)
                        *bind.target = false;
                    if (UI::event_notification) {
                        if (*bind.target) {
                            NotificationSystem_class->push(
                                std::string("Turn <color=#00FF00>ON <color=#FFFFFF>") + bind.name + "", 10, 1
                            );
                        }
                        else {
                            NotificationSystem_class->push(
                                std::string("Turn <color=#FF0000>OFF <color=#FFFFFF>") + bind.name + "", 10, 1
                            );
                        }
                    }
                    bind.last_hold_state = is_pressed;
                }
            }
        }
    }

}