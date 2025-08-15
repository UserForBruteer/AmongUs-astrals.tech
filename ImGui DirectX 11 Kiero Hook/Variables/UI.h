#pragma once
#include <unordered_map>
#include "../UI/BindInfo.hpp"
#include <Windows.h>
#include <string>
#include <d3d11.h>
#include <D3DX11core.h>
#include <D3DX11tex.h>
#include <d3dcompiler.h>

namespace UI {
	int count_pl;
	inline bool binding_menu, esp, target_strafe, task_hack, rolehack, no_cdkill, imp, ctrl_tp, speed_check, silent_kill, vanish, spam, event_notification, noclip, no_shadow, _MISHA, all_skins, esp_box, esp_dist, trail, trail_rgb, clums;
	float speed = 1.f;
	int lag = 0;
	int unlag = 1;
	inline bool startup = true;
	inline bool open = true;
	inline int bind_open = VK_INSERT;
	inline std::unordered_map<int, std::vector<BindInfo>> key_binds;
	inline bool awaiting_bind = false;
	inline bool* current_bind_target = nullptr;
	inline BindMode current_bind_mode = BindMode::Toggle;
	inline const char* current_bind_name;
	std::wstring path;
	ID3D11ShaderResourceView* logo; // with luv logo <3
	ID3D11ShaderResourceView* misha; // with luv misha <3
};