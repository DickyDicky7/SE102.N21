#pragma once

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600
#define DIRECTINPUT_VERSION 0x0800

#include <tuple>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <variant>
#include <utility>
#include <concepts>
#include <dinput.h>
#include <Windows.h>
#include <type_traits>
#include <unordered_map>

#include "BillCommon.h"
#include "SoldierCommon.h"
#include "WallTurretCommon.h"
#include "ScubaSoldierCommon.h"

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
	SOLDIER_SPRITE_ID,
        WALL_TURRET_SPRITES_ID,
	SCUBA_SOLDIER_SPRITE_ID,
>;
using TEXTURE_ID = std::variant
<
	BILL_TEXTURE_ID, 
	SOLDIER_TEXTURE_ID,
        WALL_TURRET_TEXTURE_ID,
	SCUBA_SOLDIER_TEXTURE_ID,
>;
using ANIMATION_ID = std::variant
<
	BILL_ANIMATION_ID, 
	SOLDIER_ANIMATION_ID,
        WALL_TURRET_ANIMATIONS_ID,
	SCUBA_SOLDIER_ANIMATION_ID,
>;

using         TIME = DWORD;
using DEFAULT_TIME = DWORD;

using TEXTURE   = LPDIRECT3DTEXTURE9;
using SPRITE    = std::tuple<RECT*, DIRECTION, TEXTURE_ID>;
using ANIMATION = std::tuple<DEFAULT_TIME, std::vector<std::tuple<SPRITE_ID, TIME>>>;

