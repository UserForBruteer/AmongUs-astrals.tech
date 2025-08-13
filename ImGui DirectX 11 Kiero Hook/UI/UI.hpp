#pragma once

#include "../includes.h"
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <cmath>
#include <Windows.h>
#include "../Cheats/Helpers/Helper.hpp"

#include "Keyboard.hpp"
#include "../Cheats/Rage/randcolor_all.hpp"
#include "Notification.hpp"
#include "../Cheats/Rage/rename_all.hpp"
randcolor_all* randcolor_all_class = new randcolor_all();
NotificationSystem* NotificationSystem_class = new NotificationSystem();
#include "KeyBinds.hpp"
rename_all* rename_all_class = new rename_all();
namespace menu2 {

    struct Bind {
        bool* target = nullptr;
        BindMode mode = BindMode::Toggle;
        std::string name;
    };

    struct Theme {
        ImU32 bg_tl = IM_COL32(19, 21, 28, 220);
        ImU32 bg_tr = IM_COL32(30, 32, 40, 220);
        ImU32 bg_br = IM_COL32(28, 30, 36, 220);
        ImU32 bg_bl = IM_COL32(20, 22, 28, 220);
        ImU32 accent = IM_COL32(0, 150, 255, 200);
        ImU32 accent_soft = IM_COL32(0, 150, 255, 60);
        ImU32 border = IM_COL32(255, 255, 255, 16);
        ImU32 text = IM_COL32(240, 240, 240, 255);
        ImU32 muted = IM_COL32(180, 180, 180, 200);
        float rounding = 10.0f;
        float padding = 12.0f;
        float header_h = 80.0f;
    } theme;

    inline std::unordered_map<ImGuiID, float> anim;

    inline ImU32 BlendColor(ImU32 a, ImU32 b, float t) {
        ImVec4 ca = ImGui::ColorConvertU32ToFloat4(a);
        ImVec4 cb = ImGui::ColorConvertU32ToFloat4(b);
        ImVec4 cr;
        cr.x = ca.x + (cb.x - ca.x) * t;
        cr.y = ca.y + (cb.y - ca.y) * t;
        cr.z = ca.z + (cb.z - ca.z) * t;
        cr.w = ca.w + (cb.w - ca.w) * t;
        return ImGui::ColorConvertFloat4ToU32(cr);
    }

    inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }
    std::wstring OpenFileDialog() {
        wchar_t szFile[MAX_PATH] = { 0 };

        OPENFILENAMEW ofn = { 0 };
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);

        ofn.lpstrFilter = L"Звуковые файлы (*.wav)\0*.wav\0Все файлы (*.*)\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;

        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        ofn.lpstrTitle = L"Выберите звуковой файл...";

        if (GetOpenFileNameW(&ofn) == TRUE) {
            return std::wstring(ofn.lpstrFile);
        }
        else {
            return std::wstring();
        }
    }
    inline float GetAnim(ImGuiID id) {
        auto it = anim.find(id);
        if (it == anim.end()) {
            anim[id] = 0.0f;
            return 0.0f;
        }
        return it->second;
    }
    inline void SetAnim(ImGuiID id, float v) {
        anim[id] = std::clamp(v, 0.0f, 1.0f);
    }

    inline void DrawRoundedBox(ImDrawList* dl, const ImVec2& a, const ImVec2& b, ImU32 col, ImU32 border_col, float rounding = 8.0f) {
        dl->AddRectFilled(a, b, col, rounding);
        dl->AddRect(a, b, border_col, rounding);
    }

    inline bool CustomButton(const char* id, const ImVec2& size, const ImVec2& pos = ImVec2(-1, -1)) {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImGui::SetCursorScreenPos(pos.x < 0 ? ImGui::GetCursorScreenPos() : pos);
        ImVec2 p = ImGui::GetCursorScreenPos() + ImVec2(3, 0);;
        ImGuiID gid = ImGui::GetID(id);
        bool pressed = false;
        ImGui::InvisibleButton(id, size);
        bool hovered = ImGui::IsItemHovered();
        bool down = ImGui::IsItemActive();

        float old = GetAnim(gid);
        float target = hovered ? 1.0f : 0.0f;
        float newv = Lerp(old, target, ImGui::GetIO().DeltaTime * 18.0f);
        SetAnim(gid, newv);

        ImVec2 br = ImVec2(p.x + size.x, p.y + size.y);
        ImU32 bg = BlendColor(IM_COL32(30, 30, 34, 220), theme.accent_soft, newv);
        dl->AddRectFilled(p, br, bg, 8.0f);
        dl->AddRect(p, br, theme.border, 8.0f);

        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(16.f, FLT_MAX, 0.f, id);
        ImVec2 textPos = ImVec2(p.x + (size.x - textSize.x) * 0.5f, p.y + (size.y - textSize.y) * 0.5f);
        Helper::Methods::AddText(ImGui::GetFont(), 16.0f, false, true, textPos, theme.text, id, dl);

        if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) pressed = true;
        if (UI::event_notification) {
            if (pressed) {
                NotificationSystem_class->push(
                    std::string("<color=#FF0000>") + id + " <color=#FFFFFF> tapped" + "", 2, 1
                );
            }
        }
        return pressed;
    }

    inline void CustomCheckboxWithBind(const char* label, bool* value_ptr, const char* popup_id) {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 cur = ImGui::GetCursorScreenPos() + ImVec2(3, 0);
        float fullw = ImGui::GetContentRegionAvailWidth() - 3;
        ImVec2 size = ImVec2(fullw, 34.0f);

        ImGuiID gid = ImGui::GetID(label);
        ImGui::InvisibleButton(label, size);
        bool hovered = ImGui::IsItemHovered();

        float old = GetAnim(gid);
        float target = hovered ? 1.0f : 0.0f;
        float newv = Lerp(old, target, ImGui::GetIO().DeltaTime * 18.0f);
        SetAnim(gid, newv);

        ImVec2 br = ImVec2(cur.x + size.x, cur.y + size.y);
        ImU32 bg = BlendColor(IM_COL32(24, 24, 28, 220), theme.accent_soft, newv * 0.06f);
        dl->AddRectFilled(cur, br, bg, 6.0f);
        dl->AddRect(cur, br, theme.border, 6.0f);

        ImVec2 boxP = ImVec2(cur.x + 10, cur.y + (size.y - 18) * 0.5f);
        ImVec2 boxB = ImVec2(boxP.x + 18, boxP.y + 18);
        dl->AddRectFilled(boxP, boxB, IM_COL32(26, 26, 30, 255), 4.0f);
        dl->AddRect(boxP, boxB, IM_COL32(255, 255, 255, 40), 4.0f);

        if (*value_ptr) {
            dl->AddRectFilled(ImVec2(boxP.x + 4, boxP.y + 4), ImVec2(boxB.x - 4, boxB.y - 4), theme.accent, 3.0f);
        }

        Helper::Methods::AddText(ImGui::GetFont(), 15.0f, false, true, ImVec2(cur.x + 38, cur.y + 8), theme.text, label, dl);

        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            ImGui::OpenPopup(popup_id);
        }

        if (hovered && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            *value_ptr = !*value_ptr;
            if (UI::event_notification) {
                if (*value_ptr) {
                    NotificationSystem_class->push(
                        std::string("<color=#FF0000>") + label + " <color=#FFFFFF> turned on" + "", 2, 1
                    );
                }
                else {
                    NotificationSystem_class->push(
                        std::string("<color=#FF0000>") + label + " <color=#FFFFFF> turned off" + "", 2, 1
                    );
                }
            }
        }

        if (ImGui::BeginPopup(popup_id, ImGuiWindowFlags_NoDecoration)) {
            ImVec2 popPos = ImGui::GetCursorScreenPos();
            ImVec2 popSize = ImVec2(220, 86);
            ImDrawList* pdl = ImGui::GetWindowDrawList();

            ImVec2 button_pos_1 = popPos + ImVec2(10, 8);
            ImVec2 button_pos_2 = popPos + ImVec2(10, 8 + 30 + 6);

            if (!UI::awaiting_bind) {
                if (CustomButton("Bind [Toggle]", ImVec2(200, 30), button_pos_1)) {
                    UI::awaiting_bind = true;
                    UI::current_bind_target = value_ptr;
                    UI::current_bind_mode = BindMode::Toggle;
                    UI::current_bind_name = label;
                }

                if (CustomButton("Bind [Hold]", ImVec2(200, 30), button_pos_2)) {
                    UI::awaiting_bind = true;
                    UI::current_bind_target = value_ptr;
                    UI::current_bind_mode = BindMode::Hold;
                    UI::current_bind_name = label;
                }
                ImGui::Dummy({ 220, 8 });
            }
            else {
                ImVec2 text_pos = popPos + ImVec2(12, popSize.y - 24);
                Helper::Methods::AddText(ImGui::GetFont(), 13.0f, false, true, text_pos, IM_COL32(200, 200, 200, 200), "Press a key or ESC to delete bind...", pdl);
                ImGui::Dummy(popSize);
            }

            ImGui::EndPopup();
        }
    }

    inline void CustomSliderFloat(const char* label, float* v, float v_min, float v_max) {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 cur = ImGui::GetCursorScreenPos() + ImVec2(3, 0);
        float fullw = ImGui::GetContentRegionAvailWidth() - 3;
        ImVec2 size = ImVec2(fullw, 28.0f);
        ImGuiID gid = ImGui::GetID(label);

        ImGui::InvisibleButton(label, size);
        bool hovered = ImGui::IsItemHovered();
        bool active = ImGui::IsItemActive();

        float old = GetAnim(gid);
        float target = hovered ? 1.0f : 0.0f;
        float newv = Lerp(old, target, ImGui::GetIO().DeltaTime * 18.0f);
        SetAnim(gid, newv);

        ImVec2 br = ImVec2(cur.x + size.x, cur.y + size.y);
        ImU32 bg = BlendColor(IM_COL32(28, 28, 32, 220), theme.accent_soft, newv * 0.06f);
        dl->AddRectFilled(cur, br, bg, 6.0f);
        dl->AddRect(cur, br, theme.border, 6.0f);

        ImVec2 trackP = ImVec2(cur.x + 12, cur.y + size.y * 0.5f - 6);
        ImVec2 trackB = ImVec2(cur.x + size.x - 12, cur.y + size.y * 0.5f + 6);
        dl->AddRectFilled(trackP, trackB, IM_COL32(40, 40, 44, 200), 6.0f);

        float ratio = (*v - v_min) / (v_max - v_min);
        ratio = std::clamp(ratio, 0.0f, 1.0f);
        ImVec2 filledB = ImVec2(trackP.x + (trackB.x - trackP.x) * ratio, trackB.y);
        dl->AddRectFilled(trackP, ImVec2(filledB.x, trackB.y), theme.accent, 6.0f);

        ImVec2 handleCenter = ImVec2(trackP.x + (trackB.x - trackP.x) * ratio, (trackP.y + trackB.y) * 0.5f);
        dl->AddCircleFilled(handleCenter, 8.0f, theme.accent);
        dl->AddCircle(handleCenter, 8.0f, IM_COL32(255, 255, 255, 35));

        if (active && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            float mx = ImGui::GetIO().MousePos.x;
            float rel = (mx - trackP.x) / (trackB.x - trackP.x);
            rel = std::clamp(rel, 0.0f, 1.0f);
            *v = v_min + rel * (v_max - v_min);
        }

        Helper::Methods::AddText(ImGui::GetFont(), 13.0f, false, false, ImVec2(cur.x + 14, cur.y - 18), theme.muted, label, dl);
        char buf[64]; snprintf(buf, sizeof(buf), "%.2f", *v);
        ImVec2 valPos = ImVec2(cur.x + size.x - ImGui::GetFont()->CalcTextSizeA(13.f, FLT_MAX, 0.f, buf).x - 14, cur.y - 18);
        Helper::Methods::AddText(ImGui::GetFont(), 13.0f, false, false, valPos, theme.text, buf, dl);
    }

    struct MenuRenderer {

        int selected_tab = 0;
        std::vector<std::string> tabs = { "Main", "Visual", "Rage", "Misc", "Binds" };

        static MenuRenderer& Get() {
            static MenuRenderer inst;
            return inst;
        }

        inline void AddLocalBind(int key, bool* target, BindMode mode, const std::string& name) {
            UI::key_binds[key].push_back({ target, mode, false, name.c_str()});
        }
        inline void RenderHeader(ImDrawList* dl, const ImVec2& wp, const ImVec2& ws) {
            ImVec2 br = ImVec2(wp.x + ws.x, wp.y + ws.y);
            dl->AddRectFilled(wp, br, theme.bg_tl, theme.rounding);
            dl->AddRect(wp, br, theme.border, theme.rounding);

            Helper::Methods::AddText(ImGui::GetFont(), 20.0f, true, true, ImVec2(wp.x + theme.padding, wp.y + 8.0f), theme.text, "Astral.tech", dl);

            Helper::Methods::AddText(ImGui::GetFont(), 12.0f, false, false, ImVec2(wp.x + theme.padding + 2.0f, wp.y + 30.0f), theme.muted, "an among us dlc", dl);

            ImGui::Dummy(ImVec2(0, theme.header_h));

            float tabStartX = wp.x + theme.padding;
            float tabW = (ws.x - theme.padding * 2.0f) / (float)tabs.size();
            for (size_t i = 0; i < tabs.size(); ++i) {
                ImVec2 tP = ImVec2(tabStartX + i * tabW + 4.0f, wp.y + theme.header_h - 32.0f);
                ImVec2 tB = ImVec2(tP.x + tabW - 8.0f, tP.y + 28.0f);
                ImVec2 center = ImVec2((tP.x + tB.x) * 0.5f, (tP.y + tB.y) * 0.5f);

                ImGui::SetCursorScreenPos(tP);
                char buf[64]; snprintf(buf, sizeof(buf), "tab_%zu", i);
                ImGui::InvisibleButton(buf, ImVec2(tB.x - tP.x, tB.y - tP.y));
                if (ImGui::IsItemClicked()) selected_tab = (int)i;
                bool hovered = ImGui::IsItemHovered();

                ImU32 tabBg = (selected_tab == (int)i) ? theme.accent : BlendColor(IM_COL32(0, 0, 0, 0), theme.accent_soft, hovered ? 0.08f : 0.02f);
                dl->AddRectFilled(tP, tB, (selected_tab == (int)i ? BlendColor(IM_COL32(30, 30, 34, 255), theme.accent, 0.45f) : IM_COL32(18, 18, 22, 200)), 6.0f);
                Helper::Methods::AddText(ImGui::GetFont(), 15.f, false, false, ImVec2(center.x - ImGui::GetFont()->CalcTextSizeA(15.f, FLT_MAX, 0, tabs[i].c_str()).x * 0.5f, center.y - 8.0f), theme.text, tabs[i].c_str(), dl);

                if (selected_tab == (int)i) {
                    dl->AddRectFilled(ImVec2(tP.x + 6.0f, tB.y - 4.0f), ImVec2(tB.x - 6.0f, tB.y - 1.0f), theme.accent, 2.0f);
                }
            }
        }

        inline void RenderContent(ImDrawList* dl, const ImVec2& wp, const ImVec2& ws) {
            ImGui::SetCursorScreenPos(ImVec2(wp.x + theme.padding, wp.y + theme.header_h + 6.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
            ImGui::BeginChild("##left_col", ImVec2(ws.x * 0.62f - theme.padding * 0.5f, ws.y - theme.header_h - theme.padding * 2.0f), false, ImGuiWindowFlags_NoScrollbar);

            if (selected_tab == 0) {
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("Notification", &UI::event_notification, "notif_bind");
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("No shadow", &UI::no_shadow, "nosh_bind");                
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind(u8"Михаил Сергеевич", &UI::_MISHA, "_MISHA_bind");

                ImGui::Dummy(ImVec2(3, 12));
                CustomSliderFloat("Speed", &UI::speed, 0.1f, 10.0f);

                ImGui::Dummy(ImVec2(6, 8));
                CustomCheckboxWithBind("Turn speed", &UI::speed_check, "speed_bind");

                ImGui::Dummy(ImVec2(6, 8));
                CustomCheckboxWithBind("Kick self", &UI::spam, "spam_vis");

                ImGui::SameLine();
                if (CustomButton("Set random color all", ImVec2(220, 36))) {
                    randcolor_all_class->run();
                }
                ImGui::Dummy(ImVec2(6, 8));
                if (CustomButton("Set notify sound", ImVec2(240, 36))) {
                    UI::path = OpenFileDialog();
                }
                ImGui::Dummy(ImVec2(6, 8));
                char tmp[64]; snprintf(tmp, sizeof(tmp), "Player count: %d", UI::count_pl);
                Helper::Methods::AddText(ImGui::GetFont(), 13.0f, false, false, ImVec2(ImGui::GetCursorScreenPos().x + 3, ImGui::GetCursorScreenPos().y + 6.0f), theme.muted, tmp, dl);
                ImGui::Dummy(ImVec2(6, 32));

                if (CustomButton("Unhook", ImVec2(200, 34))) {
                    kiero::shutdown();
                }

            }
            else if (selected_tab == 1) {
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("ESP", &UI::esp, "esp_bind");
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("Rolehack", &UI::rolehack, "role_bind");
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("No shadow", &UI::no_shadow, "nosh_bind_vis");
                ImGui::Dummy(ImVec2(6, 6));
            }
            else if (selected_tab == 2) {
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("Silent kill", &UI::silent_kill, "silent_bind");
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("Noclip", &UI::noclip, "noclip_bind");
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("Ctrl tp", &UI::ctrl_tp, "ctrl_tp_bind");
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("No kill distance", &UI::no_cdkill, "no_kill_dist_bind");
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("Target strafe", &UI::target_strafe, "target_strafe_bind");
            }
            else if (selected_tab == 3) {
                ImGui::Dummy(ImVec2(6, 6));
                CustomCheckboxWithBind("Vanish // IN WORK", &UI::vanish, "vanish_bind");
            }
            else if (selected_tab == 4) {
                ImGui::Dummy(ImVec2(6, 6));
                ImGui::BeginChild("##binds_list", ImVec2(0, -6), false, ImGuiWindowFlags_NoScrollbar);
                for (auto& kv : UI::key_binds) {
                    int key = kv.first;
                    for (auto& b : kv.second) {
                        if (!b.target) continue;
                        if (*b.target) {
                            std::string name = KeyBoard::GetKeyName(key);
                            if (b.name == "" || name == "Unknown")
                                continue;
                            std::string mode = (b.mode == BindMode::Toggle ? "Toggle" : "Hold");
                            char line[256]; snprintf(line, sizeof(line), "%s [%s] - %s", name.c_str(), mode.c_str(), b.name);
                            Helper::Methods::AddText(ImGui::GetFont(), 13.0f, false, false, ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 6.0f), theme.text, line, dl);
                            ImGui::Dummy(ImVec2(0, 22));
                        }
                    }
                }
                ImGui::EndChild();
            }

            ImGui::EndChild();
            
            ImGui::SameLine();
            ImGui::BeginChild("##right_col", ImVec2(ws.x * 0.36f - theme.padding * 0.5f, ws.y - theme.header_h - theme.padding * 2.0f), false, ImGuiWindowFlags_NoScrollbar);
            Helper::Methods::AddText(ImGui::GetFont(), 14.0f, false, false, ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 6.0f), theme.text, "Quick binds", dl);
            ImGui::Dummy(ImVec2(0, 8));

            for (auto& kv : UI::key_binds) {
                int key = kv.first;
                for (auto& b : kv.second) {
                    if (!b.target) continue;
                    std::string name = KeyBoard::GetKeyName(key);
                    if (b.name == "" || name == "Unknown")
                        continue;
                    char s[256]; snprintf(s, sizeof(s), "%s - %s", name.c_str(), b.name);
                    Helper::Methods::AddText(ImGui::GetFont(), 13.0f, false, false, ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + 6.0f), theme.muted, s, dl);
                    
                    ImGui::Dummy(ImVec2(0, 20));
                }
            }

            ImGui::EndChild();

            ImGui::PopStyleVar();
        }

        inline void Render() {
            NotificationSystem_class->draw(ImGui::GetFont(), 21, false, true);
            
            keybinds::process_keybinds();
            if (UI::awaiting_bind) {
                int key = -1;
                if (KeyBoard::get_current_key(key)) {
                    if (key == VK_ESCAPE) {
                        UI::awaiting_bind = false;
                        AddLocalBind(key, UI::current_bind_target, UI::current_bind_mode, UI::current_bind_name);
                        keybinds::assign_keybind(-1, UI::current_bind_target, UI::current_bind_mode);
                    }
                    else {
                        AddLocalBind(key, UI::current_bind_target, UI::current_bind_mode, UI::current_bind_name);
                        keybinds::assign_keybind(key, UI::current_bind_target, UI::current_bind_mode);
                        UI::awaiting_bind = false;
                    }
                }
            }

            if (KeyBoard::key_check_once(UI::bind_open)) UI::open = !UI::open;

            if (UI::open) return;

            ImGui::SetNextWindowSize(ImVec2(740, 560), ImGuiCond_Once);
            ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 740) * 0.5f, (ImGui::GetIO().DisplaySize.y - 560) * 0.5f), ImGuiCond_Once);
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::Begin("komaru_custom_ui", nullptr, flags);

            ImDrawList* dl = ImGui::GetWindowDrawList();
            ImVec2 wp = ImGui::GetWindowPos();
            ImVec2 ws = ImGui::GetWindowSize();

            RenderHeader(dl, wp, ws);

            ImVec2 contentA = ImVec2(wp.x + theme.padding, wp.y + theme.header_h + 6.0f);
            ImVec2 contentB = ImVec2(wp.x + ws.x - theme.padding, wp.y + ws.y - theme.padding);
            dl->AddRectFilled(contentA, contentB, IM_COL32(10, 12, 16, 255), 8.0f);

            RenderContent(dl, wp, ws);

            ImGui::End();
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
        }
    };
}

