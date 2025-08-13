#pragma once

#define uint unsigned int

#include <cstdint>
#include <libloaderapi.h>
#include "vector.hxx"
#include <vector>
#include "../../includes.h"
#include "AmongStruct.hpp"
#include "mono/string.hxx"
#include "../il2cpp/il2cpp.h"
#include <string>

class Transform;

static inline ImVec2  operator*(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
static inline ImVec2  operator/(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
static inline ImVec2  operator+(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x + rhs, lhs.y + rhs); }
static inline ImVec2  operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2  operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2  operator-(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x - rhs, lhs.y - rhs); }
static inline ImVec2  operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline ImVec2  operator/(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
static inline ImVec2& operator*=(ImVec2& lhs, const float rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const float rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
static inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
static inline ImVec2& operator*=(ImVec2& lhs, const ImVec2& rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
static inline ImVec2& operator/=(ImVec2& lhs, const ImVec2& rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }
static inline ImVec4  operator+(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
static inline ImVec4  operator-(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
static inline ImVec4  operator*(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }

namespace Helper {
	namespace Var {
		uintptr_t gameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");
		uintptr_t GameData_typeinfo = 0x29A5834;
		uintptr_t AmongUsClient_TypeInfo = 0x29835CC;
		uintptr_t PlayerControl_TypeInfo = 0x2999EC8;
		uintptr_t ShipStatus_TypeInfo = 0x29ACFE0;
		uintptr_t LocalPlayer;
		ImVec2 OnScreen_local;
		unity::vector Position_local;
		std::vector<uintptr_t> NetworkedPlayerInfo_list;
		float dist_attack = 2;
		bool needkillTimer = true;
		float real_cd;
		uintptr_t closest;
		int pl_size;
		int needback;
		bool wassss, needrename;
	};
	namespace Methods {
		unity::vector(*WorldToScreenPoint)(uintptr_t, unity::vector);
		unity::vector(*ScreenToWorldPoint)(uintptr_t, unity::vector);
		Transform*(*get_transform)(uintptr_t);
		unity::vector(*get_position)(Transform*);
		void(*set_position)(Transform*, unity::vector);
		uintptr_t(*get_current)();
		RaycastHit2D(*Raycast)(ImVec2, ImVec2, float, int);
		uintptr_t(*get_Data)(uintptr_t);
		void(*RpcSetName)(uintptr_t, void*);
		void(*CmdCheckName)(uintptr_t, void*);
		uintptr_t(*GetHost)(uintptr_t);
		void(*RpcSetColor)(uintptr_t, uintptr_t);
		typedef void* (__cdecl* il2cpp_string_new)(const char*);
		il2cpp_string_new unity_string_new;
		System_String_o*(*get_PlayerName)(uintptr_t);
		bool* (*IsNativeObjectAlive)(uintptr_t);
		void(*SetKillTimer)(uintptr_t, float);
		void(*RpcMurderPlayer)(uintptr_t, uintptr_t, bool);
		void(*RpcEnterVent)(uintptr_t, int);
		void(*RpcSendChat)(uintptr_t, void*);
		void(*RpcExitVent)(uintptr_t);
		void(*RpcSetRole)(uintptr_t, RoleTypes, bool);
		void(*set_Visible)(uintptr_t, bool);
		int(*NameToLayer)(void*);
		uintptr_t(*get_gameObject)(uintptr_t);
		void(*set_layer)(uintptr_t, int);
		uintptr_t(*GameManager_get_Instance)();
		bool(*Object_op_Implicit)(uintptr_t);
		void(*RpcCompleteTask)(uintptr_t, uint);
		void(*RpcSetScanner)(uintptr_t, bool);
		unity::vector(*get_right)(Transform*);
		unity::vector(*get_up)(Transform*);
		Transform*(*GetChild)(Transform*, int);
		Transform*(*get_parent)(Transform*);
		unity::vector(*get_localScale)(Transform*);
		inline void init() {
			WorldToScreenPoint = reinterpret_cast<unity::vector(*) (uintptr_t, unity::vector)>(Helper::Var::gameAssembly + 0x1EBF710);
			ScreenToWorldPoint = reinterpret_cast<unity::vector(*) (uintptr_t, unity::vector)>(Helper::Var::gameAssembly + 0x1EBEDB0);
			get_transform = reinterpret_cast<Transform*(*) (uintptr_t)>(Helper::Var::gameAssembly + 0x1EEF390);
			get_position = reinterpret_cast<unity::vector(*) (Transform*)>(Helper::Var::gameAssembly + 0x1F037C0);
			set_position = reinterpret_cast<void(*) (Transform*, unity::vector)>(Helper::Var::gameAssembly + 0x1F03DF0);
			get_current = reinterpret_cast<uintptr_t(*) ()>(Helper::Var::gameAssembly + 0x1EBFDC0);
			Raycast = reinterpret_cast<RaycastHit2D(*) (ImVec2, ImVec2, float, int)>(Helper::Var::gameAssembly + 0x1F3ADA0);
			get_Data = reinterpret_cast<uintptr_t(*) (uintptr_t)>(Helper::Var::gameAssembly + 0x5612D0);
			RpcSetName = reinterpret_cast<void(*) (uintptr_t, void*)>(Helper::Var::gameAssembly + 0x55E960);
			CmdCheckName = reinterpret_cast<void(*) (uintptr_t, void*)>(Helper::Var::gameAssembly + 0x5558B0);
			GetHost = reinterpret_cast<uintptr_t(*) (uintptr_t)>(Helper::Var::gameAssembly + 0x750250);
			RpcSetColor = reinterpret_cast<void(*) (uintptr_t, uintptr_t)>(Helper::Var::gameAssembly + 0x555E10);
			unity_string_new = (il2cpp_string_new)GetProcAddress((HMODULE)Helper::Var::gameAssembly, "il2cpp_string_new");
			get_PlayerName = reinterpret_cast<System_String_o*(*) (uintptr_t)>(Helper::Var::gameAssembly + 0x759F20);
			IsNativeObjectAlive = reinterpret_cast<bool*(*) (uintptr_t)>(Helper::Var::gameAssembly + 0x1EF5E50);
			SetKillTimer = reinterpret_cast<void(*) (uintptr_t, float)>(Helper::Var::gameAssembly + 0x55E770);
			RpcMurderPlayer = reinterpret_cast<void(*) (uintptr_t, uintptr_t, bool)>(Helper::Var::gameAssembly + 0x55C580);
			RpcEnterVent = reinterpret_cast<void(*) (uintptr_t, int)>(Helper::Var::gameAssembly + 0x576680);
			RpcSendChat = reinterpret_cast<void(*) (uintptr_t, void*)>(Helper::Var::gameAssembly + 0x55C970);
			RpcExitVent = reinterpret_cast<void(*) (uintptr_t)>(Helper::Var::gameAssembly + 0x575500);
			RpcSetRole = reinterpret_cast<void(*) (uintptr_t, RoleTypes, bool)>(Helper::Var::gameAssembly + 0x55D270);
			set_Visible = reinterpret_cast<void(*) (uintptr_t, bool)>(Helper::Var::gameAssembly + 0x5615D0);
			NameToLayer = reinterpret_cast<int(*) (void*)>(Helper::Var::gameAssembly + 0x1EF32A0);
			get_gameObject = reinterpret_cast<uintptr_t(*) (uintptr_t)>(Helper::Var::gameAssembly + 0x1EEF300);
			set_layer = reinterpret_cast<void(*) (uintptr_t, int)>(Helper::Var::gameAssembly + 0x1EF2610);
			GameManager_get_Instance = reinterpret_cast<uintptr_t(*) ()>(Helper::Var::gameAssembly + 0x753720);
			Object_op_Implicit = reinterpret_cast<bool(*) (uintptr_t)>(Helper::Var::gameAssembly + 0x1EF6140);
			RpcCompleteTask = reinterpret_cast<void(*) (uintptr_t, uint)>(Helper::Var::gameAssembly + 0x55C4E0);
			RpcSetScanner = reinterpret_cast<void(*) (uintptr_t, bool)>(Helper::Var::gameAssembly + 0x55D330);
			get_right = reinterpret_cast<unity::vector(*) (Transform*)>(Helper::Var::gameAssembly + 0x1F03800);
			get_up = reinterpret_cast<unity::vector(*) (Transform*)>(Helper::Var::gameAssembly + 0x1F03920);
			GetChild = reinterpret_cast<Transform*(*) (Transform*, int)>(Helper::Var::gameAssembly + 0x1F004D0);
			get_parent = reinterpret_cast<Transform*(*) (Transform*)>(Helper::Var::gameAssembly + 0x1F00600);
			get_localScale = reinterpret_cast<unity::vector(*) (Transform*)>(Helper::Var::gameAssembly + 0x1F03670);
		}

		inline ImVec2 flooring(ImVec2 vec) {
			return { (float)(vec.x), (float)int(vec.y) };
		}

		inline ImVec2 flooring(float x, float y) {
			return { (float)int(x), (float)int(y) };
		}

		inline ImVec2 flooring(int x, int y) {
			return { (float)x, (float)y };
		}


		void AddText(ImFont* font, float size, bool shadow, bool outline, const ImVec2& textpos, ImColor col, const char* value, ImDrawList* drawlist = ImGui::GetBackgroundDrawList()) {

			const char* ctext = value;

			if (outline) {
				drawlist->AddText(font, size, flooring(textpos) + ImVec2(-1, -1), ImColor(0.0f, 0.0f, 0.0f, col.Value.w * 0.75f), ctext);
				drawlist->AddText(font, size, flooring(textpos) + ImVec2(0, -1), ImColor(0.0f, 0.0f, 0.0f, col.Value.w * 0.75f), ctext);
				drawlist->AddText(font, size, flooring(textpos) + ImVec2(1, -1), ImColor(0.0f, 0.0f, 0.0f, col.Value.w * 0.75f), ctext);
				drawlist->AddText(font, size, flooring(textpos) + ImVec2(-1, 0), ImColor(0.0f, 0.0f, 0.0f, col.Value.w * 0.75f), ctext);
				drawlist->AddText(font, size, flooring(textpos) + ImVec2(1, 0), ImColor(0.0f, 0.0f, 0.0f, col.Value.w * 0.75f), ctext);
				drawlist->AddText(font, size, flooring(textpos) + ImVec2(-1, 1), ImColor(0.0f, 0.0f, 0.0f, col.Value.w * 0.75f), ctext);
				drawlist->AddText(font, size, flooring(textpos) + ImVec2(0, 1), ImColor(0.0f, 0.0f, 0.0f, col.Value.w * 0.75f), ctext);
				drawlist->AddText(font, size, flooring(textpos) + ImVec2(1, 1), ImColor(0.0f, 0.0f, 0.0f, col.Value.w * 0.75f), ctext);

			}
			if (shadow) drawlist->AddText(font, size, { textpos.x + 2, textpos.y + 2 }, ImColor(5, 5, 5, (int)float(col.Value.w * 255)), ctext);
			drawlist->AddText(font, size, textpos, col, ctext);
		}

		void print_dbg(const char* msg) {
			DWORD written;
			WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg, strlen(msg), &written, nullptr);
		}

		bool LocalInGame() {
			AmongUsClient_c* AmongUsClient = *reinterpret_cast<AmongUsClient_c**>(Helper::Var::gameAssembly + Helper::Var::AmongUsClient_TypeInfo);
			if (!AmongUsClient)
				return false;
			auto instance = AmongUsClient->static_fields->Instance;
			if (!Helper::Methods::IsNativeObjectAlive((uintptr_t)instance)) return false;
			return ((NetworkModes__Enum)instance->fields.NetworkMode == NetworkModes__Enum::LocalGame || (InnerNetClient_GameStates__Enum)instance->fields.GameState == InnerNetClient_GameStates__Enum::Started);
		}
		bool OnlineInGame() {
			AmongUsClient_c* AmongUsClient = *reinterpret_cast<AmongUsClient_c**>(Helper::Var::gameAssembly + Helper::Var::AmongUsClient_TypeInfo);
			if (!AmongUsClient)
				return false;
			auto instance = AmongUsClient->static_fields->Instance;
			if (!Helper::Methods::IsNativeObjectAlive((uintptr_t)instance)) return false;
			return ((NetworkModes__Enum)instance->fields.NetworkMode == NetworkModes__Enum::OnlineGame || (InnerNetClient_GameStates__Enum)instance->fields.GameState == InnerNetClient_GameStates__Enum::Started);
		}
		bool LocalInLobby() {
			AmongUsClient_c* AmongUsClient = *reinterpret_cast<AmongUsClient_c**>(Helper::Var::gameAssembly + Helper::Var::AmongUsClient_TypeInfo);
			if (!AmongUsClient)
				return false;
			auto instance = AmongUsClient->static_fields->Instance;
			if (!Helper::Methods::IsNativeObjectAlive((uintptr_t)instance)) return false;
			return ((NetworkModes__Enum)instance->fields.NetworkMode == NetworkModes__Enum::LocalGame || (InnerNetClient_GameStates__Enum)instance->fields.GameState == InnerNetClient_GameStates__Enum::Joined);
		}
		bool OnlineInLobby() {
			AmongUsClient_c* AmongUsClient = *reinterpret_cast<AmongUsClient_c**>(Helper::Var::gameAssembly + Helper::Var::AmongUsClient_TypeInfo);
			if (!AmongUsClient)
				return false;
			auto instance = AmongUsClient->static_fields->Instance; 
			if (!Helper::Methods::IsNativeObjectAlive((uintptr_t)instance)) return false;
			return ((NetworkModes__Enum)instance->fields.NetworkMode == NetworkModes__Enum::OnlineGame || (InnerNetClient_GameStates__Enum)instance->fields.GameState == InnerNetClient_GameStates__Enum::Joined);
		}

		bool IsInLobby() {
			AmongUsClient_c* AmongUsClient = *reinterpret_cast<AmongUsClient_c**>(Helper::Var::gameAssembly + Helper::Var::AmongUsClient_TypeInfo);
			if (!AmongUsClient)
				return false;
			auto instance = AmongUsClient->static_fields->Instance;
			if (!Helper::Methods::IsNativeObjectAlive((uintptr_t)instance)) return false;
			if (!Helper::Methods::GameManager_get_Instance()) return false;
			return (LocalInLobby() || OnlineInLobby()) && Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer);
		}

		bool IsInGame() {
			AmongUsClient_c* AmongUsClient = *reinterpret_cast<AmongUsClient_c**>(Helper::Var::gameAssembly + Helper::Var::AmongUsClient_TypeInfo);
			if (!AmongUsClient)
				return false;
			auto instance = AmongUsClient->static_fields->Instance;

			uintptr_t ShipStatus_TypeInfo = *reinterpret_cast<uintptr_t*>(Helper::Var::gameAssembly + Helper::Var::ShipStatus_TypeInfo);
			if (!ShipStatus_TypeInfo)
				return false;

			uintptr_t ShipStatus_static = *reinterpret_cast<uintptr_t*>(ShipStatus_TypeInfo + 0x5C);
			if (!ShipStatus_static)
				return false;
			uintptr_t ShipStatus_Instance = *reinterpret_cast<uintptr_t*>(ShipStatus_static + 0x0);
			if (!ShipStatus_Instance)
				return false;

			if (Helper::Methods::IsNativeObjectAlive((uintptr_t)instance)) return false;
			if (!Helper::Methods::GameManager_get_Instance()) return false;
			return (LocalInGame() || OnlineInGame()) && Helper::Methods::IsNativeObjectAlive(ShipStatus_Instance) && Helper::Methods::IsNativeObjectAlive(Helper::Var::LocalPlayer);
		}
		std::string SystemTypeToString(SystemTypes type) {
			size_t index = static_cast<size_t>(type);

			if (index < SystemTypeNames.size()) {
				return SystemTypeNames[index];
			}
			return "Unknown";
		};
	};
};

class Transform {
public:
	uintptr_t GetPointer() {
		return (uintptr_t)this;
	}

	unity::vector GetPosition() {
		return Helper::Methods::get_position(this);
	}

	void SetPosition(unity::vector pos) {
		Helper::Methods::set_position(this, pos);
	}

	unity::vector GetRight() {
		return Helper::Methods::get_right(this);
	}

	unity::vector GetUp() {
		return Helper::Methods::get_up(this);
	}

	unity::vector get_localScale() {
		return Helper::Methods::get_localScale(this);
	}

	Transform* GetChild(int index) {
		return Helper::Methods::GetChild(this, index);
	}

	Transform* GetParent() {
		return Helper::Methods::get_parent(this);
	}

	inline bool operator==(Transform& rhs) {
		return this->GetPointer() == rhs.GetPointer();
	}

	inline bool operator!=(Transform& rhs) {
		return this->GetPointer() != rhs.GetPointer();
	}
};
