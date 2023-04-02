#include "Bill.h"

Bill::Bill() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	self = this;

	this->vx = 1;
	this->vy = 1;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = SCREEN_WIDTH / 4;
	this->position.y = SCREEN_HEIGHT / 2 - 50;

	this->updateState = NULL;
	this->handleInputState = NULL;
	this->state = new BillNormalState();
}

Bill::~Bill()
{
}

/// <summary>
/// this fuction is check is the Bill hit the wall left and right ?
/// </summary>
/// <returns></returns>
BOOL Bill::IsHitWall() {
	BOOL isHited = 0;
	// check is hit the wall left
	bool isHitLeftWall = (GetX() <= 0) && (GetDirection() == DIRECTION::LEFT);
	bool isHitRightWall = (GetX() >= SCREEN_WIDTH - 520) && (GetDirection() == DIRECTION::RIGHT);

	if (!isHitLeftWall && !isHitRightWall) isHited = 1;

	return isHited;
}

void Bill::Update()
{
	updateState = state->Update(*this);
}

void Bill::Render()
{
	state->Render(*this);
	if (updateState)
	{
		state->Exit(*this);
		delete state;
		state = updateState;
		state->Enter(*this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		state->Exit(*this);
		delete state;
		state = handleInputState;
		state->Enter(*this);
		handleInputState = NULL;
	}
}

void Bill::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void Bill::LoadSprites()
{
	if (HasSprites<Bill>::hasBeenLoaded.value) return;
	HasSprites<Bill>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::NORMAL_01,  30,  1, 25, 65, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::NORMAL_SHOT_01,  30,  1, 25, 65, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::NORMAL_SHOT_02,  30,  1, 25, 65, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::STRAIGHT_UP_01,  19, 51, 66, 65, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SHOT_STRAIGHT_UP_01,  19, 51, 66, 65, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SHOT_STRAIGHT_UP_02,  19, 67, 82, 65, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::LAYDOWN_01,  48, 83,116, 65, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::JUMP_01,  44,117,134, 65, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::JUMP_02,  44,135,155, 65, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::JUMP_03,  44,156,173, 65, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::JUMP_04,  44,174,194, 65, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_01,  66,  2, 23,102, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_02,  66, 24, 41,102, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_03,  66, 42, 61,102, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_04,  66, 62, 83,102, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_05,  66, 84,101,102, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_06,  66,102,122,102, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_01,  66,123,144,102, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_02,  66,145,162,102, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_03,  66,163,183,102, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_01,  66,184,206,102, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_02,  66,207,228,102, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_03,  66,229,251,102, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DEAD_01, 107,  2, 18,131, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DEAD_02, 112, 19, 42,131, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DEAD_03, 107, 43, 59,131, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DEAD_04, 120, 60, 93,131, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::BEGIN_SWIM_01, 115, 94,111,131, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::DIVE_01, 123,112,129,131, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_01, 115,130,147,131, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_SHOT_ANGLE_UP_01, 113,148,168,131, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_SHOT_STRAIGHT_UP_01, 103,169,187,131, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::SWIM_SHOT_01, 113,191,217,131, BILL_TEXTURE_ID::BILL_01);

	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_01, 189,155,181,224, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_02, 189,184,208,224, BILL_TEXTURE_ID::BILL_01);
	GraphicsHelper::InsertSprite(BILL_SPRITE_ID::RUN_SHOT_03, 189,210,236,224, BILL_TEXTURE_ID::BILL_01);

#pragma endregion Load Sprites

	OutputDebugString(L"Bill Sprites Loaded Successfully\n");
}

void Bill::LoadTextures()
{
	if (HasTextures<Bill>::hasBeenLoaded.value) return;
	HasTextures<Bill>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(BILL_TEXTURE_ID::BILL_01, L"Resources\\Textures\\MainCharacter2.bmp");

	OutputDebugString(L"Bill Textures Loaded Successfully\n");
}

void Bill::LoadAnimations()
{
	if (HasAnimations<Bill>::hasBeenLoaded.value) return;
	HasAnimations<Bill>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::NORMAL, 150,
	{
		{BILL_SPRITE_ID::NORMAL_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::NORMAL_SHOT, 150,
	{
		{BILL_SPRITE_ID::NORMAL_SHOT_01,0},
		{BILL_SPRITE_ID::NORMAL_SHOT_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::STRAIGHT_UP, 150,
	{
		{BILL_SPRITE_ID::STRAIGHT_UP_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SHOT_STRAIGHT_UP, 150,
	{
		{BILL_SPRITE_ID::SHOT_STRAIGHT_UP_01,0},
		{BILL_SPRITE_ID::SHOT_STRAIGHT_UP_02,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::LAYDOWN, 150,
	{
		{BILL_SPRITE_ID::LAYDOWN_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::JUMP, 60,
	{
		{BILL_SPRITE_ID::JUMP_01,0},
		{BILL_SPRITE_ID::JUMP_02,0},
		{BILL_SPRITE_ID::JUMP_03,0},
		{BILL_SPRITE_ID::JUMP_04,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN, 70,
	{
		{BILL_SPRITE_ID::RUN_01,0},
		{BILL_SPRITE_ID::RUN_02,0},
		{BILL_SPRITE_ID::RUN_03,0},
		{BILL_SPRITE_ID::RUN_04,0},
		{BILL_SPRITE_ID::RUN_05,0},
		{BILL_SPRITE_ID::RUN_06,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN_SHOT_ANGLE_UP, 150,
	{
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_01,0},
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_02,0},
		{BILL_SPRITE_ID::RUN_SHOT_ANGLE_UP_03,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN_SHOT_ANGLE_DOWN, 150,
		{
			{BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_01,0},
			{BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_02,0},
			{BILL_SPRITE_ID::RUN_SHOT_ANGLE_DOWN_03,0},
		});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::DEAD, 150,
	{
		{BILL_SPRITE_ID::DEAD_01,0},
		{BILL_SPRITE_ID::DEAD_02,0},
		{BILL_SPRITE_ID::DEAD_03,0},
		{BILL_SPRITE_ID::DEAD_04,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::BEGIN_SWIM, 150,
	{
		{BILL_SPRITE_ID::BEGIN_SWIM_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::DIVE, 150,
	{
		{BILL_SPRITE_ID::DIVE_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM, 150,
	{
		{BILL_SPRITE_ID::SWIM_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_SHOT_ANGLE_UP, 150,
	{
		{BILL_SPRITE_ID::SWIM_SHOT_ANGLE_UP_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_SHOT_STRAIGHT_UP, 150,
	{
		{BILL_SPRITE_ID::SWIM_SHOT_STRAIGHT_UP_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::SWIM_SHOT, 150,
	{
		{BILL_SPRITE_ID::SWIM_SHOT_01,0},
	});

	GraphicsHelper::InsertAnimation(BILL_ANIMATION_ID::RUN_SHOT, 150,
	{
		{BILL_SPRITE_ID::RUN_SHOT_01,0},
		{BILL_SPRITE_ID::RUN_SHOT_02,0},
		{BILL_SPRITE_ID::RUN_SHOT_03,0},
	});

#pragma endregion Load Animations

	OutputDebugString(L"Bill Animations Loaded Successfully\n");
}