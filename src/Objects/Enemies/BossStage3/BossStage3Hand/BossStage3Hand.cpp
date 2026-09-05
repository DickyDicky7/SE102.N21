#include "BossStage3Hand.h"
#include "Explosion.h"

BossStage3Hand::BossStage3Hand() : Entity(), HasTextures(), HasSprites(), HasAnimations(), HasWeapons(new BulletBossStage2StateHand())
{
	this->_w = Constants::Enemies::BossStage3::Hand::JOINT_WIDTH;
	this->_h = Constants::Enemies::BossStage3::Hand::JOINT_HEIGHT;

	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Enemies::BossStage3::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::BossStage3::DEFAULT_SPAWN_Y;
	this->SetDebugName(L"BossStage3Hand\n");

	this->_updateState = nullptr;
	this->_handleInputState = nullptr;
	// set direction default is right
	this->_movingDirection = DIRECTION::RIGHT;
	// set state begin is run
	this->_state = new BossStage3HandStartState(*this);

	this->_isInitPositionJoints = true;

	this->InitPositionJoints();

	this->_hitCounts = Constants::Enemies::BossStage3::Hand::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::BOSS;

	this->_isFire = false;

	this->SetFiringRate(Constants::Enemies::BossStage3::Hand::FIRING_RATE_MILLISECONDS);
}

BossStage3Hand::~BossStage3Hand()
{
	this->_isFire = false;
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);

	for (size_t i = 0; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		Destroy(this->_joints[i]);
	}
	this->_isInitPositionJoints = false;
}

void BossStage3Hand::InitPositionJoints()
{
	for (size_t i = 0; i < Constants::Enemies::BossStage3::Hand::ARM_JOINTS_COUNT; i++)
	{
		this->_joints[i] = new BossStage3Joint(BOSS_STAGE_3_HAND_ANIMATION_ID::ARM, this->_position, this->_movingDirection, this);
	}
	this->_joints[Constants::Enemies::BossStage3::Hand::HAND_JOINT_INDEX] = new BossStage3Joint(BOSS_STAGE_3_HAND_ANIMATION_ID::HAND, this->_position, this->_movingDirection, this);
}

void BossStage3Hand::Update()
{
	if (this->IsDead())
	{
		Sound::GetInstance()->Play("boss2finalhanddisappear", false, 1);
	}

	if (!this->_isInitPositionJoints)
	{
		this->InitPositionJoints();
		this->_state->Enter(*this);
		this->_isInitPositionJoints = true;
	}
	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void BossStage3Hand::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void BossStage3Hand::HandleInput(Input& input)
{
	DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
}

void InsertSpriteBoss3Hand(SPRITE_ID spriteId, int left, int top, int right, int bottom)
{
	// i write this function to shorten the function: GraphicsHelper
	GraphicsHelper::InsertSprite(spriteId, top, left, right, bottom, DIRECTION::RIGHT, ROCK_FLY_TEXTURE_ID::ROCK_FLY);
}

void BossStage3Hand::LoadSprites()
{
	if (HasSprites<BossStage3Hand>::_hasBeenLoaded) return;
	HasSprites<BossStage3Hand>::_hasBeenLoaded = true;

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
	if (HasAnimations<BossStage3Hand>::_hasBeenLoaded) return;
	HasAnimations<BossStage3Hand>::_hasBeenLoaded = true;

#pragma region Load Animations

	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_HAND_ANIMATION_ID::ARM, Constants::Enemies::BossStage3::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_3_HAND_SPRITE_ID::ARM_01,0},
		});

	GraphicsHelper::InsertAnimation(BOSS_STAGE_3_HAND_ANIMATION_ID::HAND, Constants::Enemies::BossStage3::ANIMATION_DELAY_MILLISECONDS,
		{
			{BOSS_STAGE_3_HAND_SPRITE_ID::HAND_01,0},
		});

#pragma endregion Load Animations

	OutputDebugString(L"BossStage3Hand Animations Loaded Successfully\n");
}

void BossStage3Hand::Fire(float x, float y, float vx, float vy)
{
	this->HasWeapons::Fire(x, y, 0.0f, vx, vy, 0.0f, 0.0f, this->_movingDirection);
}

void BossStage3Hand::Fire()
{
	this->HasWeapons::Fire(this->_position.x, this->_position.y, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, this->_movingDirection);
}

void BossStage3Hand::ForEachCollisionEntity(std::function<void(Entity*)> callback)
{
	callback(this);
	for (size_t i = 0; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		if (this->_joints[i]) callback(this->_joints[i]);
	}
}

void BossStage3Hand::ProcessSpecialDeathEffects(std::vector<Entity*>& effectEntities)
{
	Sound::GetInstance()->Play("boss2finalhanddisappear", false, 1);
	for (size_t i = 0; i < Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT; i++)
	{
		if (this->_joints[i])
		{
			Explosion* subExplosion = new Explosion(new ExplosionType3State());
			subExplosion->SetX(this->_joints[i]->GetX());
			subExplosion->SetY(this->_joints[i]->GetY());
			effectEntities.push_back(subExplosion);
		}
	}
}
