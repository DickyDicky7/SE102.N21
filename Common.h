#pragma once

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 600
#define SCALING_RATIO_X 2.5f
#define SCALING_RATIO_Y 2.5f
#define DIRECTINPUT_VERSION 0x0800

#include <any>
#include <tuple>
#include <queue>
#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <d3d9.h>
#include <d3dx9.h>
#include <variant>
#include <utility>
#include <concepts>
#include <dinput.h>
#include <optional>
#include <iterator>
#include <Windows.h>
#include <functional>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include "FireCommon.h"
#include "BillCommon.h"
#include "SceneCommon.h"
#include "LetterCommon.h"
#include "BulletCommon.h"
#include "FalconCommon.h"
#include "RockFlyCommon.h"
#include "SoldierCommon.h"
#include "AirCraftCommon.h"
#include "RockFallCommon.h"
#include "RifleManCommon.h"
#include "WallTurretCommon.h"
#include "CannonCommon.h"
#include "BossStage3Common.h"
#include "BossStage3GateCommon.h"
#include "BossStage3HandCommon.h"
#include "ScubaSoldierCommon.h"
#include "TerrainStage1Common.h"
#include "TerrainStage2Common.h"
#include "TerrainStage1Common.h"

enum class DIRECTION
{

	LEFT,
	RIGHT,

};

template <class T>
struct Bool { BOOL value; };

// If u add an object, u must add id of object here
using SPRITE_ID = std::variant
<
	BILL_SPRITE_ID,
	FIRE_SPRITE_ID,
	SCENE_SPRITE_ID,
	LETTER_SPRITE_ID,
	FALCON_SPRITE_ID,
	BULLET_SPRITE_ID,
	SOLDIER_SPRITE_ID,
	AIRCRAFT_SPRITE_ID,
	ROCK_FLY_SPRITE_ID,
	ROCK_FALL_SPRITE_ID,
	RIFLE_MAN_SPRITE_ID,
	WALL_TURRET_SPRITE_ID,
	BOSS_STAGE_3_SPRITE_ID,
	BOSS_STAGE_3_HAND_SPRITE_ID,
	BOSS_STAGE_3_GATE_SPRITE_ID,
	SCUBA_SOLDIER_SPRITE_ID,
	TERRAIN_STAGE_1_SPRITE_ID,
	TERRAIN_STAGE_2_SPRITE_ID,
	CANNON_SPRITE_ID
>;

using TEXTURE_ID = std::variant
<
	BILL_TEXTURE_ID,
	FIRE_TEXTURE_ID,
	SCENE_TEXTURE_ID,
	LETTER_TEXTURE_ID,
	FALCON_TEXTURE_ID,
	BULLET_TEXTURE_ID,
	SOLDIER_TEXTURE_ID,
	AIRCRAFT_TEXTURE_ID,
	ROCK_FLY_TEXTURE_ID,
	ROCK_FALL_TEXTURE_ID,
	RIFLE_MAN_TEXTURE_ID,
	WALL_TURRET_TEXTURE_ID,
	BOSS_STAGE_3_TEXTURE_ID,
	SCUBA_SOLDIER_TEXTURE_ID,
	TERRAIN_STAGE_1_TEXTURE_ID,
	TERRAIN_STAGE_2_TEXTURE_ID,
	CANNON_TEXTURE_ID
>;

using ANIMATION_ID = std::variant
<
	BILL_ANIMATION_ID,
	FIRE_ANIMATION_ID,
	SCENE_ANIMATION_ID,
	LETTER_ANIMATION_ID,
	FALCON_ANIMATION_ID,
	BULLET_ANIMATION_ID,
	SOLDIER_ANIMATION_ID,
	AIRCRAFT_ANIMATION_ID,
	ROCK_FLY_ANIMATION_ID,
	ROCK_FALL_ANIMATION_ID,
	RIFLE_MAN_ANIMATION_ID,
	WALL_TURRET_ANIMATION_ID,
	BOSS_STAGE_3_ANIMATION_ID,
	BOSS_STAGE_3_HAND_ANIMATION_ID,
	BOSS_STAGE_3_GATE_ANIMATION_ID,
	SCUBA_SOLDIER_ANIMATION_ID,
	TERRAIN_STAGE_1_ANIMATION_ID,
	TERRAIN_STAGE_2_ANIMATION_ID,
	CANNON_ANIMATION_ID
>;

using         TIME = DWORD;
using DEFAULT_TIME = DWORD;

using TEXTURE   = LPDIRECT3DTEXTURE9;
using SPRITE    = std::tuple<RECT*, DIRECTION, TEXTURE_ID>;
using ANIMATION = std::tuple<DEFAULT_TIME, std::vector<std::tuple<SPRITE_ID, TIME>>>;

//using KEYBOARD_EVENT_HANDLER = std::function<void(LPDIRECTINPUTDEVICE8, char(&)[256])>;
//using    MOUSE_EVENT_HANDLER = std::function<void(LPDIRECTINPUTDEVICE8, DIMOUSESTATE&)>;

template <class T>
inline void Destroy(T*& pointer)
{
	if (pointer)
	{
		delete pointer;
		pointer = NULL;
	}
}

template <int prefix> class ID : public std::string { public: ID(std::string value) : std::string(std::to_string(prefix) + " " + value) {} };

inline std::string FormatId(std::string id)
{
	return id.size() == 1 ? "00" + id : id.size() == 2 ? "0" + id : id;
}

