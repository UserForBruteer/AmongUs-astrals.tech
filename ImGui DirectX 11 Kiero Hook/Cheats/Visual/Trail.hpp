#pragma once

#include "../../includes.h"
#include "../il2cpp/il2cpp.h"
#include "../Helpers/mono/mono_ellements.hxx"
#include "../Helpers/Helper.hpp"
#include <iostream>
#include <vector>

struct TrailPoint {
    unity::vector Pos;
    float spawnTime;
    ImColor color;
};

static std::vector<TrailPoint> trailPoints;

void AddTrailPoint(const unity::vector& Pos, const ImColor& color) {
    const float eps = 0.001f;

    for (auto& p : trailPoints) {
        if (fabs(p.Pos.x - Pos.x) < eps &&
            fabs(p.Pos.y - Pos.y) < eps &&
            fabs(p.Pos.z - Pos.z) < eps) {
            return;
        }
    }

    trailPoints.push_back({ Pos, (float)ImGui::GetTime(), color });
}

// Генерация цвета радуги по времени
ImColor GetRainbowColor(float speed = 1.0f, float time = 0) {
    float t = ((float)ImGui::GetTime() + time) * speed;
    float hue = fmodf(t, 1.0f);
    ImVec4 col = ImColor::HSV(hue, 1.0f, 1.0f);
    return ImColor(col);
}

class trail {
public:
    void draw() {
        if (!Helper::Var::LocalPlayer || !Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer) ||
            !Helper::Methods::get_current() || !Helper::Methods::IsNativeObjectAlive(Helper::Methods::get_current()))
            return;

        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
        const float lifeTime = 1.0f; // секунда
        float now = (float)ImGui::GetTime();

        for (size_t i = 0; i < trailPoints.size();) {
            float age = now - trailPoints[i].spawnTime;
            if (age >= lifeTime) {
                trailPoints.erase(trailPoints.begin() + i);
                continue;
            }

            float alpha = 1.0f - (age / lifeTime); // от 1 до 0

            // Если включен RGB режим
            ImColor faded = UI::trail_rgb ? GetRainbowColor(0.5f, trailPoints[i].spawnTime) : trailPoints[i].color;
            faded.Value.w *= alpha; // плавное угасание

            unity::vector screen = Helper::Methods::WorldToScreenPoint(Helper::Methods::get_current(), trailPoints[i].Pos);
            if (screen.z > 0.01f) {
                ++i;
                continue;
            }

            if (i + 1 < trailPoints.size()) {
                unity::vector screen2 = Helper::Methods::WorldToScreenPoint(Helper::Methods::get_current(), trailPoints[i + 1].Pos);
                if (screen2.z > 0.01f) {
                    ++i;
                    continue;
                }
                ImGui::GetBackgroundDrawList()->AddLine(
                    { screen.x, screenSize.y - screen.y },
                    { screen2.x, screenSize.y - screen2.y },
                    ImColor(faded.Value.x, faded.Value.y, faded.Value.z, alpha),
                    3
                );
            }

            ++i;
        }
    }
};
