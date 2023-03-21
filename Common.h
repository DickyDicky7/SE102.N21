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

enum SPRITE_ID
{

	BILL_NORMAL_01,
	BILL_NORMAL_02,

	BILL_RUN_01,
	BILL_RUN_02,
	BILL_RUN_03,
	BILL_RUN_04,
	BILL_RUN_05,
	BILL_RUN_06,

	BILL_JUMP_01,
	BILL_JUMP_02,
	BILL_JUMP_03,
	BILL_JUMP_04,

	BILL_LAYDOWN_01,

	BILL_RUN_SHOT_ANGLE_UP_01,
	BILL_RUN_SHOT_ANGLE_UP_02,
	BILL_RUN_SHOT_ANGLE_UP_03,

	BILL_RUN_SHOT_ANGLE_DOWN_01,
	BILL_RUN_SHOT_ANGLE_DOWN_02,
	BILL_RUN_SHOT_ANGLE_DOWN_03,

	BILL_SHOT_STRAIGHT_UP_01,
	BILL_SHOT_STRAIGHT_UP_02,

	BILL_RUN_SHOT_01,
	BILL_RUN_SHOT_02,
	BILL_RUN_SHOT_03,

	BILL_BEGIN_SWIM_01,
	BILL_SWIM_01,
	BILL_DIVE_01,
	BILL_SWIM_SHOT_01,
	BILL_SWIM_SHOT_ANGLE_UP_01,
	BILL_SWIM_SHOT_STRAIGHT_UP_01,

	BILL_DEAD_01,
	BILL_DEAD_02,
	BILL_DEAD_03,
	BILL_DEAD_04,

};

enum TEXTURE_ID
{

	BILL,

};

enum ANIMATION_ID
{

	BILL_NORMAL,
	BILL_RUN,
	BILL_JUMP,
	BILL_LAYDOWN,
	BILL_RUN_SHOT_ANGLE_UP,
	BILL_RUN_SHOT_ANGLE_DOWN,
	BILL_SHOT_STRAIGHT_UP,
	BILL_RUN_SHOT,
	BILL_BEGIN_SWIM,
	BILL_SWIM,
	BILL_DIVE,
	BILL_SWIM_SHOT,
	BILL_SWIM_SHOT_ANGLE_UP,
	BILL_SWIM_SHOT_STRAIGHT_UP,
	BILL_DEAD,

};

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

enum DIRECTION
{
	LEFT, RIGHT,
};