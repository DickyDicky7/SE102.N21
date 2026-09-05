#pragma once

#include "Constants.h"

// <dinput.h> selects its ABI from this macro at preprocessing time, so this one
// value cannot be a constant.  Constants::Input::DIRECT_INPUT_VERSION mirrors it
// and Input.cpp static_asserts the two against each other, so they cannot drift.
#define DIRECTINPUT_VERSION 0x0800

#include <any>
#include <tuple>
#include <queue>
#include <vector>
#include <string>
#include <d3d11.h>
#include "DX11Math.h"
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
#include "ItemCommon.h"
#include "BillCommon.h"
#include "SceneCommon.h"
#include "BridgeCommon.h"
#include "CannonCommon.h"
#include "LetterCommon.h"
#include "BulletCommon.h"
#include "FalconCommon.h"
#include "RockFlyCommon.h"
#include "SoldierCommon.h"
#include "AirCraftCommon.h"
#include "RockFallCommon.h"
#include "RifleManCommon.h"
#include "ExplosionCommon.h"
#include "WallTurretCommon.h"
#include "BossStage1Common.h"
#include "BossStage3Common.h"
#include "ScubaSoldierCommon.h"
#include "TerrainStage1Common.h"
#include "TerrainStage2Common.h"
#include "BossStage3GateCommon.h"
#include "BossStage3HandCommon.h"

enum class DIRECTION
{

	LEFT,
	RIGHT,

};

enum class TERRAIN_BLOCK_TYPE
{
	NONE,
	WALL,
	WATER,
	CHECK_POINT,
	THROUGHABLE,
	NON_THROUGHABLE,
};

enum class ENEMY_TYPE
{
	NONE,
	BOSS,
	HUMAN,
	MACHINE,
};

class Entity;

struct AABBSweepResult
{
	bool isCollided = false;

	float enTime   = -std::numeric_limits<float>::infinity();
	float exTime   = +std::numeric_limits<float>::infinity();
	float normalX  = +std::numeric_limits<float>::infinity();
	float normalY  = +std::numeric_limits<float>::infinity();
	float contactX = +std::numeric_limits<float>::infinity();
	float contactY = +std::numeric_limits<float>::infinity();

	Entity* surfaceEntity = nullptr;
};

// If u add an object, u must add id of object here
using SPRITE_ID = std::variant
<
	ITEM_SPRITE_ID,
	BILL_SPRITE_ID,
	FIRE_SPRITE_ID,
	SCENE_SPRITE_ID,
	BRIDGE_SPRITE_ID,
	CANNON_SPRITE_ID,
	LETTER_SPRITE_ID,
	FALCON_SPRITE_ID,
	BULLET_SPRITE_ID,
	SOLDIER_SPRITE_ID,
	AIRCRAFT_SPRITE_ID,
	ROCK_FLY_SPRITE_ID,
	ROCK_FALL_SPRITE_ID,
	RIFLE_MAN_SPRITE_ID,
	EXPLOSION_SPRITE_ID,
	WALL_TURRET_SPRITE_ID,
	BOSS_STAGE_1_SPRITE_ID,
	BOSS_STAGE_3_SPRITE_ID,
	SCUBA_SOLDIER_SPRITE_ID,
	TERRAIN_STAGE_1_SPRITE_ID,
	TERRAIN_STAGE_2_SPRITE_ID,
	BOSS_STAGE_3_HAND_SPRITE_ID,
	BOSS_STAGE_3_GATE_SPRITE_ID
>;

using TEXTURE_ID = std::variant
<
	BILL_TEXTURE_ID,
	FIRE_TEXTURE_ID,
	SCENE_TEXTURE_ID,
	BRIDGE_TEXTURE_ID,
	CANNON_TEXTURE_ID,
	LETTER_TEXTURE_ID,
	FALCON_TEXTURE_ID,
	BULLET_TEXTURE_ID,
	SOLDIER_TEXTURE_ID,
	AIRCRAFT_TEXTURE_ID,
	ROCK_FLY_TEXTURE_ID,
	ROCK_FALL_TEXTURE_ID,
	RIFLE_MAN_TEXTURE_ID,
	EXPLOSION_TEXTURE_ID,
	WALL_TURRET_TEXTURE_ID,
	BOSS_STAGE_1_TEXTURE_ID,
	BOSS_STAGE_3_TEXTURE_ID,
	SCUBA_SOLDIER_TEXTURE_ID,
	TERRAIN_STAGE_1_TEXTURE_ID,
	TERRAIN_STAGE_2_TEXTURE_ID
>;

using ANIMATION_ID = std::variant
<
	ITEM_ANIMATION_ID,
	BILL_ANIMATION_ID,
	FIRE_ANIMATION_ID,
	SCENE_ANIMATION_ID,
	BRIDGE_ANIMATION_ID,
	CANNON_ANIMATION_ID,
	LETTER_ANIMATION_ID,
	FALCON_ANIMATION_ID,
	BULLET_ANIMATION_ID,
	SOLDIER_ANIMATION_ID,
	AIRCRAFT_ANIMATION_ID,
	ROCK_FLY_ANIMATION_ID,
	ROCK_FALL_ANIMATION_ID,
	RIFLE_MAN_ANIMATION_ID,
	EXPLOSION_ANIMATION_ID,
	WALL_TURRET_ANIMATION_ID,
	BOSS_STAGE_1_ANIMATION_ID,
	BOSS_STAGE_3_ANIMATION_ID,
	SCUBA_SOLDIER_ANIMATION_ID,
	TERRAIN_STAGE_1_ANIMATION_ID,
	TERRAIN_STAGE_2_ANIMATION_ID,
	BOSS_STAGE_3_HAND_ANIMATION_ID,
	BOSS_STAGE_3_GATE_ANIMATION_ID
>;

using TIME = DWORD;

// D3D11 texture wrapper: SRV + dimensions for UV computation
struct TEXTURE
{
	ID3D11ShaderResourceView* srv;
	UINT                     width;
	UINT                     height;

	TEXTURE() : srv(nullptr), width(0), height(0) {}
	TEXTURE(ID3D11ShaderResourceView* s, UINT w, UINT h) : srv(s), width(w), height(h) {}
};
using SPRITE    = std::tuple<RECT, DIRECTION, TEXTURE_ID>;
using ANIMATION = std::tuple<TIME, std::vector<std::tuple<SPRITE_ID, TIME>>>;

template <class T>
inline void Destroy(T*& pointer)
{
	if (pointer)
	{
		delete pointer;
		pointer = nullptr;
	}
}

template <int prefix>
class ID : public std::string
{
public:
	ID(std::string_view value)
	{
		std::string prefixStr = std::to_string(prefix);
		this->reserve(prefixStr.size() + 1 + value.size());
		this->append(prefixStr);
		this->push_back(' ');
		this->append(value);
	}
	ID(const char* value) : ID(std::string_view(value)) {}
	ID(const std::string& value) : ID(std::string_view(value)) {}
};

inline std::string FormatId(std::string_view id)
{
	if (id.size() == 1) { std::string s; s.reserve(4); s.append("00"); s.append(id); return s; }
	if (id.size() == 2) { std::string s; s.reserve(4); s.append("0"); s.append(id); return s; }
	return std::string(id);
}
