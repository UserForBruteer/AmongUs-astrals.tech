#pragma once
#include "../../includes.h" 
#include <string>
struct RaycastHit2D {
	ImVec2 m_Centroid; // 0x0
	ImVec2 m_Point; // 0x8
	ImVec2 m_Normal; // 0x10
	float m_Distance; // 0x18
	float m_Fraction; // 0x1C
	int m_Collider; // 0x20
};

struct Color {
	float r; // 0x0
	float g; // 0x4
	float b; // 0x8
	float a; // 0xC

};

enum RoleTypes : byte {
    Crewmate = 0,
    Impostor = 1,
    Scientist = 2,
    Engineer = 3,
    GuardianAngel = 4,
    Shapeshifter = 5,
    CrewmateGhost = 6,
    ImpostorGhost = 7,
    Noisemaker = 8,
    Phantom = 9,
    Tracker = 10
};

enum class InnerNetClient_GameStates__Enum : int32_t
{
    NotJoined = 0x00000000,
    Joined = 0x00000001,
    Started = 0x00000002,
    Ended = 0x00000003,
};

enum class NetworkModes__Enum : int32_t {
    LocalGame = 0x00000000,
    OnlineGame = 0x00000001,
    FreePlay = 0x00000002,
};

enum SystemTypes : byte {
    Hallway = 0, Storage = 1, Cafeteria = 2, Reactor = 3, UpperEngine = 4,
    Nav = 5, Admin = 6, Electrical = 7, LifeSupp = 8, Shields = 9,
    MedBay = 10, Security = 11, Weapons = 12, LowerEngine = 13, Comms = 14,
    ShipTasks = 15, Doors = 16, Sabotage = 17, Decontamination = 18, Launchpad = 19,
    LockerRoom = 20, Laboratory = 21, Balcony = 22, Office = 23, Greenhouse = 24,
    Dropship = 25, Decontamination2 = 26, Outside = 27, Specimens = 28, BoilerRoom = 29,
    VaultRoom = 30, Cockpit = 31, Armory = 32, Kitchen = 33, ViewingDeck = 34,
    HallOfPortraits = 35, CargoBay = 36, Ventilation = 37, Showers = 38, Engine = 39,
    Brig = 40, MeetingRoom = 41, Records = 42, Lounge = 43, GapRoom = 44,
    MainHall = 45, Medical = 46, Decontamination3 = 47, Zipline = 48, MiningPit = 49,
    FishingDock = 50, RecRoom = 51, Lookout = 52, Beach = 53, Highlands = 54,
    Jungle = 55, SleepingQuarters = 56, MushroomMixupSabotage = 57, HeliSabotage = 58
};

const std::vector<std::string> SystemTypeNames = {
    "Hallway", "Storage", "Cafeteria", "Reactor", "UpperEngine", "Nav",
    "Admin", "Electrical", "LifeSupp", "Shields", "MedBay", "Security",
    "Weapons", "LowerEngine", "Comms", "ShipTasks", "Doors", "Sabotage",
    "Decontamination", "Launchpad", "LockerRoom", "Laboratory", "Balcony",
    "Office", "Greenhouse", "Dropship", "Decontamination2", "Outside",
    "Specimens", "BoilerRoom", "VaultRoom", "Cockpit", "Armory", "Kitchen",
    "ViewingDeck", "HallOfPortraits", "CargoBay", "Ventilation", "Showers",
    "Engine", "Brig", "MeetingRoom", "Records", "Lounge", "GapRoom", "MainHall",
    "Medical", "Decontamination3", "Zipline", "MiningPit", "FishingDock",
    "RecRoom", "Lookout", "Beach", "Highlands", "Jungle", "SleepingQuarters",
    "MushroomMixupSabotage", "HeliSabotage"
};

