#include "BossStage3Hand.h"

BossStage3Hand::BossStage3Hand() : Entity(), HasTextures(), HasSprites(), HasAnimations(), HasWeapons(new BulletBossStage2StateHand())
{
	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 200;
	this->position.y = 200;
	this->name = L"BossStage3Hand\n";

	this->updateState = NULL;
	this->handleInputState = NULL;
	// set direction default is right
	this->movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->state = new BossStage3HandStartState(*this);

	isInitPositionJoints = false;

	initPositionJoints();

	this->hitCounts = 10;
	this->enemyType = ENEMY_TYPE::BOSS;

	this->isFire = false;

	SetFiringRate(500);
}

BossStage3Hand::~BossStage3Hand()
{
	isFire = NULL;
	Destroy(state);
	Destroy(updateState);
	Destroy(handleInputState);

	for (size_t i = 0; i < 5; i++)
	{
		Destroy(joints[i]);
	}
	isInitPositionJoints = NULL;
}

void BossStage3Hand::initPositionJoints()
{
	for (size_t i = 0; i < 4; i++)
	{
		joints[i] = new BossStage3Joint(BOSS_STAGE_3_HAND_ANIMATION_ID::ARM, position, movingDirection);
	}
	joints[4] = new BossStage3Joint(BOSS_STAGE_3_HAND_ANIMATION_ID::HAND, position, movingDirection);
}

void BossStage3Hand::Update()
{
	if (!isInitPositionJoints) 
	{
		initPositionJoints();
		this->state->Enter(*this);
		isInitPositionJoints = true;
	}
	updateState = state->Update(*this);
}

void BossStage3Hand::Render()
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

void BossStage3Hand::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void InsertSpriteBoss3Hand(SPRITE_ID spriteId, INT left, INT top, INT right, INT bottom)
{
	// i write this function to shorten the fuction: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, ROCK_FLY_TEXTURE_ID::ROCK_FLY);
}

void BossStage3Hand::LoadSprites()
{
	if (HasSprites<BossStage3Hand>::hasBeenLoaded.value) return;
	HasSprites<BossStage3Hand>::hasBeenLoaded.value = true;

#pragma region Load Sprites

	// SPRITES

	InsertSpriteBoss3Hand(BOSS_STAGE_3_HAND_SPRITE_ID::HAND_01, 67, 17, 82, 32);
	InsertSpriteBoss3Hand(BOSS_STAGE_3_HAND_SPRITE_ID::ARM_01,  86, 17, 101, 32);

#pragma endregion Load Sprites

	OutputDebugString(L"BossStage3Hand Sprites Loaded Successfully\n");
}

void BossStage3Hand::LoadTextures()
{
	// is loaded
}

void BossStage3Hand::LoadAnimations()
{
	if (HasAnimations<BossStage3Hand>::hasBeenLoaded.value) return;
	HasAnimations<BossStage3Hand>::hasBeenLoaded.value = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_HAND_ANIMATION_ID::ARM, 150,
		{
			{BOSS_STAGE_3_HAND_SPRITE_ID::ARM_01,0},
		});
	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_HAND_ANIMATION_ID::HAND, 150,
		{
			{BOSS_STAGE_3_HAND_SPRITE_ID::HAND_01,0},
		});
#pragma endregion Load Animations

	OutputDebugString(L"BossStage3Hand Animations Loaded Successfully\n");
}

void BossStage3Hand::Fire(FLOAT x, FLOAT y, FLOAT vx, FLOAT vy)
{
	HasWeapons::Fire(x, y, 0.0f, vx, vy, 0.0f, 0.0f, movingDirection);
}

void BossStage3Hand::Fire()
{
	HasWeapons::Fire(position.x, position.y, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, movingDirection);
}