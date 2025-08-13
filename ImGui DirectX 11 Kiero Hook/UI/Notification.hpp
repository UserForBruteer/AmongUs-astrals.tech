#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <regex>
#include <sstream>
#include "../includes.h"
#include "../Cheats/Helpers/Helper.hpp"
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
struct Notification {
    std::string text;
    float lifetime;
    float time_created;
    float fade_time;

    Notification(const std::string& msg, float life = 3.f, float fade = 0.5f)
        : text(msg), lifetime(life), fade_time(fade)
    {
        time_created = get_time_sec();
    }

    bool is_alive() const {
        return (get_time_sec() - time_created) < lifetime + fade_time;
    }

    float alpha() const {
        float t = get_time_sec() - time_created;
        if (t < lifetime) return 1.f;
        float fade = (lifetime + fade_time - t) / fade_time;
        return fade < 0.f ? 0.f : fade;
    }

    static float get_time_sec() {
        using namespace std::chrono;
        return duration_cast<duration<float>>(steady_clock::now().time_since_epoch()).count();
    }
};

class NotificationSystem {
public:
    __declspec(dllexport) void PlaySimpleSound(const wchar_t* soundFilePath) {
        PlaySoundW(soundFilePath, NULL, SND_FILENAME | SND_ASYNC);
    }

    void push(const std::string& msg, float life = 3.f, float fade = 0.5f) {
        if (notifications.size() >= MAX_NOTIFICATIONS) {
            notifications.erase(notifications.begin());
        }


        notifications.emplace_back(msg, life, fade);

        if (!UI::path.empty()) {
            PlaySimpleSound(UI::path.c_str());
        }
    }

    void draw(ImFont* font, float size = 16.f, bool shadow = true, bool outline = false) {
        ImVec2 display = ImGui::GetIO().DisplaySize;

        float x_center = display.x * 0.5f;
        float y_start = (display.y * 0.5f) + (display.y * 0.25f);

        float y = y_start;
        for (auto it = notifications.begin(); it != notifications.end();) {
            if (!it->is_alive()) {
                it = notifications.erase(it);
                continue;
            }

            float a = it->alpha();
            float text_width = font->CalcTextSizeA(size, FLT_MAX, 0.0f, std::regex_replace(it->text, std::regex("<[^>]+>"), "").c_str()).x;

            ImVec2 pos(x_center - (text_width * 0.5f), y);
            draw_parsed_text(font, size, shadow, outline, pos, it->text, a);

            y += size + 4.f;
            ++it;
        }
    }


private:
    int MAX_NOTIFICATIONS = 5;
    std::vector<Notification> notifications;
    void draw_parsed_text(ImFont* font, float size, bool shadow, bool outline, const ImVec2& pos, const std::string& text, float alpha) {
        std::regex color_tag("<color=#([0-9A-Fa-f]{6})>");
        std::sregex_iterator it(text.begin(), text.end(), color_tag);
        std::sregex_iterator end;

        ImVec2 cursor = pos;
        ImColor current_color = ImColor(255, 255, 255, int(alpha * 255));

        size_t last_pos = 0;
        for (; it != end; ++it) {
            auto match = *it;
            size_t tag_pos = match.position();
            if (tag_pos > last_pos) {
                std::string chunk = text.substr(last_pos, tag_pos - last_pos);
                Helper::Methods::AddText(font, size, shadow, outline, cursor, current_color, chunk.c_str());
                cursor.x += font->CalcTextSizeA(size, FLT_MAX, 0.0f, chunk.c_str()).x;
            }

            std::string hex = match.str(1);
            current_color = ImColor(
                std::stoi(hex.substr(0, 2), nullptr, 16),
                std::stoi(hex.substr(2, 2), nullptr, 16),
                std::stoi(hex.substr(4, 2), nullptr, 16),
                int(alpha * 255)
            );

            last_pos = tag_pos + match.length();
        }

        if (last_pos < text.size()) {
            std::string chunk = text.substr(last_pos);
            Helper::Methods::AddText(font, size, shadow, outline, cursor, current_color, chunk.c_str());
        }
    }
};
