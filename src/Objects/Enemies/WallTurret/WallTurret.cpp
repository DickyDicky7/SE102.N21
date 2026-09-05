#include "WallTurret.h"

void AutoIncreasePositionSpriteLoader(
	const std::vector<std::pair<int, int>>&,
	const std::vector<WALL_TURRET_SPRITE_ID>&,
	const TEXTURE_ID&
);
void AutoIncreaseSpriteIdLoadAnimations(const std::vector<WALL_TURRET_SPRITE_ID>&, const ANIMATION_ID&);

WallTurret::WallTurret() : Entity(), HasAnimations(), HasWeapons(new BulletEnemyState())
{
	this->SetDebugName(L"WallTurret\n");

	this->_vx = Constants::Physics::DEFAULT_INITIAL_VELOCITY_X;
	this->_vy = Constants::Physics::DEFAULT_INITIAL_VELOCITY_Y;
	this->_ax = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_X;
	this->_ay = Constants::Physics::DEFAULT_INITIAL_ACCELERATION_Y;
	this->_position.x = Constants::Enemies::WallTurret::DEFAULT_SPAWN_X;
	this->_position.y = Constants::Enemies::WallTurret::DEFAULT_SPAWN_Y;


	this->_movingDirection = DIRECTION::LEFT;

	this->_updateState = nullptr;
	this->_state = nullptr;

	this->_hitCounts = Constants::Enemies::WallTurret::HEALTH_POINTS;
	this->_enemyType = ENEMY_TYPE::MACHINE;
	this->ResetShootDelay();
}

WallTurret::~WallTurret()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
}

void WallTurret::Update() {
	if (!this->_state)
	{
		this->_state = new WallTurretNormalState();
	}

	if (this->_target && !this->_target->IsDead())
		DeferState(this->_updateState, this->_state->Update(*this));

	// As in Cannon: firing lives in the states' Enter, so a step with no
	// transition re-enters the current state, and the countdown that gates it now
	// runs on the logic clock instead of once per rendered frame.
	if (this->_updateState)
	{
		this->ResetShootDelay();

		ApplyDeferredState(this->_state, this->_updateState, this);
	}
	else
	{
		this->_state->Enter(*this);
	}
}

void WallTurret::Render() {
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void WallTurret::HandleInput(Input& input) {}

float WallTurret::CalculateBillAngle()
{
	return this->CalculateTargetAngle(this);
}

bool WallTurret::IsTargetInRange() const
{
	if (!this->_target) return false;
	float dx = std::abs(this->GetX() - this->_target->GetX());

	return dx <= Constants::Screen::TILE_SIZE * Constants::Enemies::WallTurret::TARGET_RANGE_TILES;
}

void WallTurret::LoadTextures() {
	if (HasTextures<WallTurret>::_hasBeenLoaded) {
		return;
	}
	HasTextures<WallTurret>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(WALL_TURRET_TEXTURE_ID::WALL_TURRET, L"Resources\\Textures\\WallTurretAll.bmp");
	return;
}

void WallTurret::LoadSprites() {

	if (HasSprites<WallTurret>::_hasBeenLoaded) {
		return;
	}
	HasSprites<WallTurret>::_hasBeenLoaded = true;

#pragma region WALL_TURRET_LEFT_30 SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 0, 0 }, { 0, 32 }, { 0, 64 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_30_01, WALL_TURRET_SPRITE_ID::LEFT_30_02, WALL_TURRET_SPRITE_ID::LEFT_30_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_LEFT_60 SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 32, 0 }, { 32, 32 }, { 32, 64 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_60_01, WALL_TURRET_SPRITE_ID::LEFT_60_02, WALL_TURRET_SPRITE_ID::LEFT_60_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_LEFT_90 SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 64, 0 }, { 64, 32 }, { 64, 64 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_90_01, WALL_TURRET_SPRITE_ID::LEFT_90_02, WALL_TURRET_SPRITE_ID::LEFT_90_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_LEFT_120 SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 96, 0 }, { 96, 32 }, { 96, 64 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_120_01, WALL_TURRET_SPRITE_ID::LEFT_120_02, WALL_TURRET_SPRITE_ID::LEFT_120_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_LEFT_150 SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 128, 0 }, { 128, 32 }, { 128, 64 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_150_01, WALL_TURRET_SPRITE_ID::LEFT_150_02, WALL_TURRET_SPRITE_ID::LEFT_150_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_RIGHT_30 SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 0, 192 }, { 0, 224 }, { 0, 256 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_30_01, WALL_TURRET_SPRITE_ID::RIGHT_30_02, WALL_TURRET_SPRITE_ID::RIGHT_30_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_RIGHT_60 SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 32, 192 }, { 32, 224 }, { 32, 256 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_60_01, WALL_TURRET_SPRITE_ID::RIGHT_60_02, WALL_TURRET_SPRITE_ID::RIGHT_60_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_RIGHT_90 SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 64, 192 }, { 64, 224 }, { 64, 256 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_90_01, WALL_TURRET_SPRITE_ID::RIGHT_90_02, WALL_TURRET_SPRITE_ID::RIGHT_90_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_RIGHT_120 SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 96, 192 }, { 96, 224 }, { 96, 256 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_120_01, WALL_TURRET_SPRITE_ID::RIGHT_120_02, WALL_TURRET_SPRITE_ID::RIGHT_120_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_RIGHT_150 SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 128, 96 }, { 128, 128 }, { 128, 160 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_150_01, WALL_TURRET_SPRITE_ID::RIGHT_150_02, WALL_TURRET_SPRITE_ID::RIGHT_150_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_UP SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 0, 96 }, { 0, 128 }, { 0, 160 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::UP_01, WALL_TURRET_SPRITE_ID::UP_02, WALL_TURRET_SPRITE_ID::UP_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_DOWN SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 96, 96 }, { 96, 128 }, { 96, 160 }},
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::DOWN_01, WALL_TURRET_SPRITE_ID::DOWN_02, WALL_TURRET_SPRITE_ID::DOWN_03},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
	);

#pragma endregion

#pragma region WALL_TURRET_APPEAR SPRITE

	AutoIncreasePositionSpriteLoader(
		std::vector<std::pair<int, int>>{ { 64, 96 }, { 64, 128 }, { 64, 160 }, { 32, 96 }, { 32, 128 }, { 32, 160 }},
		std::vector<WALL_TURRET_SPRITE_ID>
	{
		WALL_TURRET_SPRITE_ID::APPEAR_01,
			WALL_TURRET_SPRITE_ID::APPEAR_02,
			WALL_TURRET_SPRITE_ID::APPEAR_03,
			WALL_TURRET_SPRITE_ID::APPEAR_04,
			WALL_TURRET_SPRITE_ID::APPEAR_05,
			WALL_TURRET_SPRITE_ID::APPEAR_06,
	},
		WALL_TURRET_TEXTURE_ID::WALL_TURRET
			);

#pragma endregion

}

void WallTurret::LoadAnimations() {

	if (HasAnimations<WallTurret>::_hasBeenLoaded) {
		return;
	}
	HasAnimations<WallTurret>::_hasBeenLoaded = true;

#pragma region LOAD LEFT ANIMATIONS

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_30_01, WALL_TURRET_SPRITE_ID::LEFT_30_02, WALL_TURRET_SPRITE_ID::LEFT_30_03},
		WALL_TURRET_ANIMATION_ID::LEFT_30
	);

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_60_01, WALL_TURRET_SPRITE_ID::LEFT_60_02, WALL_TURRET_SPRITE_ID::LEFT_60_03},
		WALL_TURRET_ANIMATION_ID::LEFT_60
	);

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_90_01, WALL_TURRET_SPRITE_ID::LEFT_90_02, WALL_TURRET_SPRITE_ID::LEFT_90_03},
		WALL_TURRET_ANIMATION_ID::LEFT_90
	);

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_120_01, WALL_TURRET_SPRITE_ID::LEFT_120_02, WALL_TURRET_SPRITE_ID::LEFT_120_03},
		WALL_TURRET_ANIMATION_ID::LEFT_120
	);

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_150_01, WALL_TURRET_SPRITE_ID::LEFT_150_02, WALL_TURRET_SPRITE_ID::LEFT_150_03},
		WALL_TURRET_ANIMATION_ID::LEFT_150
	);

#pragma endregion

#pragma region LOAD RIGHT ANIMATIONS

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_30_01, WALL_TURRET_SPRITE_ID::RIGHT_30_02, WALL_TURRET_SPRITE_ID::RIGHT_30_03},
		WALL_TURRET_ANIMATION_ID::RIGHT_30
	);

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_60_01, WALL_TURRET_SPRITE_ID::RIGHT_60_02, WALL_TURRET_SPRITE_ID::RIGHT_60_03},
		WALL_TURRET_ANIMATION_ID::RIGHT_60
	);

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_90_01, WALL_TURRET_SPRITE_ID::RIGHT_90_02, WALL_TURRET_SPRITE_ID::RIGHT_90_03},
		WALL_TURRET_ANIMATION_ID::RIGHT_90
	);

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_120_01, WALL_TURRET_SPRITE_ID::RIGHT_120_02, WALL_TURRET_SPRITE_ID::RIGHT_120_03},
		WALL_TURRET_ANIMATION_ID::RIGHT_120
	);

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_150_01, WALL_TURRET_SPRITE_ID::RIGHT_150_02, WALL_TURRET_SPRITE_ID::RIGHT_150_03},
		WALL_TURRET_ANIMATION_ID::RIGHT_150
	);

#pragma endregion

#pragma region LOAD UP ANIMATIONS

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::UP_01, WALL_TURRET_SPRITE_ID::UP_02, WALL_TURRET_SPRITE_ID::UP_03},
		WALL_TURRET_ANIMATION_ID::UP
	);

#pragma endregion

#pragma region LOAD DOWN ANIMATIONS

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::DOWN_01, WALL_TURRET_SPRITE_ID::DOWN_02, WALL_TURRET_SPRITE_ID::DOWN_03},
		WALL_TURRET_ANIMATION_ID::DOWN
	);

#pragma endregion

#pragma region LOAD OPENING ANIMATIONS

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>
	{
		WALL_TURRET_SPRITE_ID::APPEAR_01,
			WALL_TURRET_SPRITE_ID::APPEAR_02,
			WALL_TURRET_SPRITE_ID::APPEAR_03,
			WALL_TURRET_SPRITE_ID::APPEAR_04,
			WALL_TURRET_SPRITE_ID::APPEAR_05,
			WALL_TURRET_SPRITE_ID::APPEAR_06,
	},
		WALL_TURRET_ANIMATION_ID::OPENING);

#pragma endregion

#pragma region LOAD CLOSING ANIMATIONS

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>
	{
		WALL_TURRET_SPRITE_ID::APPEAR_06,
			WALL_TURRET_SPRITE_ID::APPEAR_05,
			WALL_TURRET_SPRITE_ID::APPEAR_04,
			WALL_TURRET_SPRITE_ID::APPEAR_03,
			WALL_TURRET_SPRITE_ID::APPEAR_02,
			WALL_TURRET_SPRITE_ID::APPEAR_01,
	},
		WALL_TURRET_ANIMATION_ID::CLOSING);

#pragma endregion

#pragma region LOAD NORMAL ANIMATIONS

	AutoIncreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>
	{
		WALL_TURRET_SPRITE_ID::APPEAR_01,
			WALL_TURRET_SPRITE_ID::APPEAR_02,
			WALL_TURRET_SPRITE_ID::APPEAR_03,
	},
		WALL_TURRET_ANIMATION_ID::NORMAL);

#pragma endregion
}

#pragma region Helpers

void AutoIncreasePositionSpriteLoader(
	const std::vector<std::pair<int, int>>& topRightConnerOfSpritePositions,
	const std::vector<WALL_TURRET_SPRITE_ID>& spriteIdList,
	const TEXTURE_ID& textureId
)
{
	constexpr int turretW = static_cast<int>(Constants::Enemies::WallTurret::WIDTH);
	constexpr int turretH = static_cast<int>(Constants::Enemies::WallTurret::HEIGHT);

	for (int i = 0; std::cmp_less(i, topRightConnerOfSpritePositions.size()); i++)
	{
		GraphicsHelper::InsertSprite(
			spriteIdList[i],
			topRightConnerOfSpritePositions[i].first,
			topRightConnerOfSpritePositions[i].second,
			topRightConnerOfSpritePositions[i].second + turretW,
			topRightConnerOfSpritePositions[i].first + turretH,
			DIRECTION::LEFT,
			textureId
		);
	}
}

void AutoIncreaseSpriteIdLoadAnimations(const std::vector<WALL_TURRET_SPRITE_ID>& spriteIdList, const ANIMATION_ID& animationId)
{
	std::vector<std::tuple<SPRITE_ID, DWORD>> listOfSpriteWithRefreshRate;
	listOfSpriteWithRefreshRate.reserve(spriteIdList.size());

	for (int i = 0; std::cmp_less(i, spriteIdList.size()); i++)
	{
		listOfSpriteWithRefreshRate.push_back(std::make_tuple(spriteIdList[i], 0));
	}

	GraphicsHelper::InsertAnimation(animationId, Constants::Enemies::WallTurret::ANIMATION_DELAY_MILLISECONDS, std::move(listOfSpriteWithRefreshRate));
}

#pragma endregion


void WallTurret::Fire()
{
	if (!Enemy::_target || Enemy::_target->IsDead())
	{
		return;
	}
}

void WallTurret::Fire(float angle, float vx, float vy, float ax, float ay, DIRECTION direction)
{
	if (!Enemy::_target || Enemy::_target->IsDead())
	{
		return;
	}
	HasWeapons::Fire(this->_position.x, this->_position.y + this->_h / 2, angle, vx, vy, ax, ay, direction);
}
