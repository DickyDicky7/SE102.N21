#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <vector>
#include <d3d9.h>
#include <d3dx9.h>
#include <utility>
#include <concepts>
#include <dinput.h>
#include <Windows.h>
#include <type_traits>
#include <unordered_map>

enum SOLDIER_SPRITE_ID
{
	SOLDIER_RUN_01,
	SOLDIER_RUN_02,
	SOLDIER_RUN_03,
	SOLDIER_RUN_04,
	SOLDIER_RUN_05,
	SOLDIER_RUN_06,

	SOLDIER_JUMP_01,

	SOLDIER_SHOOT_01,
	SOLDIER_SHOOT_02,

	SOLDIER_LAY_DOWN_01,

	SOLDIER_DIE_01,
};

enum SOLDIER_ANIMATION_ID
{
	SOLDIER_RUN,
	SOLDIER_JUMP,
	SOLDIER_SHOOT,
	SOLDIER_LAY_DOWN,
	SOLDIER_DIE,
};

enum SOLDIER_DIRECTION
{
	LEFT, RIGHT,
};