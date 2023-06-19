#include "BossStage3Gate.h"

BossStage3Gate::BossStage3Gate() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 200;
	this->position.y = 200;
	this->name = L"BossStage3Gate\n";

	this->updateState = NULL;
	this->handleInputState = NULL;
	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->state = new BossStage3GateCloseState();
}

BossStage3Gate::~BossStage3Gate()
{
	Destroy(state);
	Destroy(updateState);
	Destroy(handleInputState);
}

void BossStage3Gate::Update()
{
	if (IsDead()) this->isDead = true;
	updateState = state->Update(*this);
}

void BossStage3Gate::Render()
{
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		ChangeState(state, updateState, this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		ChangeState(state, handleInputState, this);
		handleInputState = NULL;
	}
}

void BossStage3Gate::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteBoss3Gate(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, BOSS_STAGE_3_TEXTURE_ID::BOSS_STAGE_3);
}

void BossStage3Gate::LoadSprites()
{
	if (HasSprites<BossStage3Gate>::hasBeenLoaded.value) return;
	HasSprites<BossStage3Gate>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteBoss3Gate(BOSS_STAGE_3_GATE_SPRITE_ID::CLOSE_1, 338, 96, 338 + 68, 96 + 85);
	InsertSpriteBoss3Gate(BOSS_STAGE_3_GATE_SPRITE_ID::OPEN_1, 444, 96, 444 + 68, 96 + 85);

#pragma endregion Load Sprites

	OutputDebugString(L"BossStage3Gate Sprites Loaded Successfully\n");
}

void BossStage3Gate::LoadTextures()
{
	// ís loaded
}

void BossStage3Gate::LoadAnimations()
{
	if (HasAnimations<BossStage3Gate>::hasBeenLoaded.value) return;
	HasAnimations<BossStage3Gate>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_GATE_ANIMATION_ID::OPEN, 150,
		{
			{BOSS_STAGE_3_GATE_SPRITE_ID::OPEN_1,0},
		});
	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_GATE_ANIMATION_ID::CLOSE, 150,
		{
			{BOSS_STAGE_3_GATE_SPRITE_ID::CLOSE_1,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"BossStage3Gate Animations Loaded Successfully\n");
}

BOOL BossStage3Gate::IsDead()
{
	return bossStage3Head->isDead;
}