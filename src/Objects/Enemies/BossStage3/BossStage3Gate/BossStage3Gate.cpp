#include "BossStage3Gate.h"

BossStage3Gate::BossStage3Gate() : Entity(), HasTextures(), HasSprites(), HasAnimations()
{
	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Enemies::BossStage3::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::BossStage3::DEFAULT_SPAWN_Y;
	this->SetDebugName(L"BossStage3Gate\n");

	this->_updateState = nullptr;
	this->_handleInputState = nullptr;
	// Filled in by SetHead; IsDead reads it unguarded.
	this->_bossStage3Head = nullptr;
	// set direction default is right
	this->_movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->_state = new BossStage3GateCloseState();
}

BossStage3Gate::~BossStage3Gate()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);
}

void BossStage3Gate::Update()
{
	if (!this->IsDead() && this->IsHeadDead())
	{
		this->SetDead(true);
		Sound::GetInstance()->Play("boss2finaldestroy", false, 1);
	}
	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void BossStage3Gate::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void BossStage3Gate::HandleInput(Input& input)
{
	DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
}

void InsertSpriteBoss3Gate(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, BOSS_STAGE_3_TEXTURE_ID::BOSS_STAGE_3);
}

void BossStage3Gate::LoadSprites()
{
	if (HasSprites<BossStage3Gate>::_hasBeenLoaded) return;
	HasSprites<BossStage3Gate>::_hasBeenLoaded = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteBoss3Gate(BOSS_STAGE_3_GATE_SPRITE_ID::CLOSE_1, 338, 96, 338 + 68, 96 + 85);
	InsertSpriteBoss3Gate(BOSS_STAGE_3_GATE_SPRITE_ID::OPEN_1, 444, 96, 444 + 68, 96 + 85);

#pragma endregion Load Sprites

	OutputDebugString(L"BossStage3Gate Sprites Loaded Successfully\n");
}

void BossStage3Gate::LoadTextures()
{
	// The gate draws from BOSS_STAGE_3, which BossStage3 loads; nothing to do here.
}

void BossStage3Gate::LoadAnimations()
{
	if (HasAnimations<BossStage3Gate>::_hasBeenLoaded) return;
	HasAnimations<BossStage3Gate>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_GATE_ANIMATION_ID::OPEN, Constants::Enemies::BossStage3::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_3_GATE_SPRITE_ID::OPEN_1,0},
		});
	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_GATE_ANIMATION_ID::CLOSE, Constants::Enemies::BossStage3::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_3_GATE_SPRITE_ID::CLOSE_1,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"BossStage3Gate Animations Loaded Successfully\n");
}

bool BossStage3Gate::IsHeadDead() const
{
	return this->_bossStage3Head && this->_bossStage3Head->IsDead();
}
