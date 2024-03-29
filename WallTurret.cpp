#include "WallTurret.h"

#define PI D3DX_PI
#define	TURRET_HEIGHT 32
#define TURRET_WIDTH 32

void AutoIncreasePositionSpriteLoader(
	std::vector<std::pair<int, int>>,
	std::vector<WALL_TURRET_SPRITE_ID>,
	TEXTURE_ID
);
void AutoInscreaseSpriteIdLoadAnimations(std::vector<WALL_TURRET_SPRITE_ID>, ANIMATION_ID);

WallTurret::WallTurret() : Entity(), HasAnimations(), HasWeapons(new BulletEnemyState())
{
	this->name = L"WallTurret\n";

	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 50;
	this->position.y = 50;


	this->movingDirection = DIRECTION::LEFT;

	this->updateState = NULL;
	this->state = NULL;

	this->hitCounts = 10;
	this->enemyType = ENEMY_TYPE::MACHINE;
	shootDelay = WALL_TURRET_SHOOT_DELAY;
}

WallTurret::~WallTurret() {}

void WallTurret::Update() {
	if (!state)
	{
		state = new WallTurretNormalState();
	}

	if (!Enemy::target->isDead)
		updateState = state->Update(*this);
}

void WallTurret::Render() {
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		shootDelay = WALL_TURRET_SHOOT_DELAY;

		ChangeState(state, updateState, this);
		updateState = NULL;
		return;
	}
	state->Enter(*this);
}

void WallTurret::HandleInput(Input& input) {}

FLOAT WallTurret::CalculateBillAngle()
{
	float dx = +(this->GetPosition().x - Enemy::target->GetPosition().x);
	float dy = -(this->GetPosition().y - Enemy::target->GetPosition().y);

	if (dx > 0 && dy < 0)
		return (-atan(dx / (abs(dy))) * 180 / PI);
	else if (dx < 0 && dy < 0)
		return (atan(abs(dx) / abs(dy)) * 180 / PI);
	else if (dx > 0 && dy > 0)
		return (atan(dx / dy) * 180 / PI - 180);
	else if (dx < 0 && dy > 0)
		return (-atan(abs(dx) / dy) * 180 / PI + 180);

	return -90.0f;
}

BOOLEAN WallTurret::IsTargetInRange()
{
	float dx = abs(this->GetX() - Enemy::target->GetX());

	return dx <= 16.0f * 6;
}

void WallTurret::LoadTextures() {
	if (HasTextures<WallTurret>::hasBeenLoaded.value) {
		return;
	}
	HasTextures<WallTurret>::hasBeenLoaded.value = true;

	GraphicsDatabase::textures.insert({ WALL_TURRET_TEXTURE_ID::WALL_TURRET, GraphicsHelper::CreateTexture(L"Resources\\Textures\\WallTurretAll.bmp") });

	return;
}

void WallTurret::LoadSprites() {

	if (HasSprites<WallTurret>::hasBeenLoaded.value) {
		return;
	}
	HasSprites<WallTurret>::hasBeenLoaded.value = true;

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

	if (HasAnimations<WallTurret>::hasBeenLoaded.value) {
		return;
	}
	HasAnimations<WallTurret>::hasBeenLoaded.value = true;

#pragma region LOAD LEFT ANIMATIONS

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_30_01, WALL_TURRET_SPRITE_ID::LEFT_30_02, WALL_TURRET_SPRITE_ID::LEFT_30_03},
		WALL_TURRET_ANIMATION_ID::LEFT_30
	);

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_60_01, WALL_TURRET_SPRITE_ID::LEFT_60_02, WALL_TURRET_SPRITE_ID::LEFT_60_03},
		WALL_TURRET_ANIMATION_ID::LEFT_60
	);

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_90_01, WALL_TURRET_SPRITE_ID::LEFT_90_02, WALL_TURRET_SPRITE_ID::LEFT_90_03},
		WALL_TURRET_ANIMATION_ID::LEFT_90
	);

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_120_01, WALL_TURRET_SPRITE_ID::LEFT_120_02, WALL_TURRET_SPRITE_ID::LEFT_120_03},
		WALL_TURRET_ANIMATION_ID::LEFT_120
	);

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::LEFT_150_01, WALL_TURRET_SPRITE_ID::LEFT_150_02, WALL_TURRET_SPRITE_ID::LEFT_150_03},
		WALL_TURRET_ANIMATION_ID::LEFT_150
	);

#pragma endregion

#pragma region LOAD RIGHT ANIMATIONS

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_30_01, WALL_TURRET_SPRITE_ID::RIGHT_30_02, WALL_TURRET_SPRITE_ID::RIGHT_30_03},
		WALL_TURRET_ANIMATION_ID::RIGHT_30
	);

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_60_01, WALL_TURRET_SPRITE_ID::RIGHT_60_02, WALL_TURRET_SPRITE_ID::RIGHT_60_03},
		WALL_TURRET_ANIMATION_ID::RIGHT_60
	);

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_90_01, WALL_TURRET_SPRITE_ID::RIGHT_90_02, WALL_TURRET_SPRITE_ID::RIGHT_90_03},
		WALL_TURRET_ANIMATION_ID::RIGHT_90
	);

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_120_01, WALL_TURRET_SPRITE_ID::RIGHT_120_02, WALL_TURRET_SPRITE_ID::RIGHT_120_03},
		WALL_TURRET_ANIMATION_ID::RIGHT_120
	);

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::RIGHT_150_01, WALL_TURRET_SPRITE_ID::RIGHT_150_02, WALL_TURRET_SPRITE_ID::RIGHT_150_03},
		WALL_TURRET_ANIMATION_ID::RIGHT_150
	);

#pragma endregion

#pragma region LOAD UP ANIMATIONS

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::UP_01, WALL_TURRET_SPRITE_ID::UP_02, WALL_TURRET_SPRITE_ID::UP_03},
		WALL_TURRET_ANIMATION_ID::UP
	);

#pragma endregion

#pragma region LOAD DOWN ANIMATIONS

	AutoInscreaseSpriteIdLoadAnimations(
		std::vector<WALL_TURRET_SPRITE_ID>{WALL_TURRET_SPRITE_ID::DOWN_01, WALL_TURRET_SPRITE_ID::DOWN_02, WALL_TURRET_SPRITE_ID::DOWN_03},
		WALL_TURRET_ANIMATION_ID::DOWN
	);

#pragma endregion

#pragma region LOAD OPENING ANIMATIONS

	AutoInscreaseSpriteIdLoadAnimations(
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

	AutoInscreaseSpriteIdLoadAnimations(
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

	AutoInscreaseSpriteIdLoadAnimations(
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
	std::vector<std::pair<int, int>> topRightConnerOfSpritePositions,
	std::vector<WALL_TURRET_SPRITE_ID> spriteIdList,
	TEXTURE_ID textureId
)
{
	for (int i = 0; std::cmp_less(i, topRightConnerOfSpritePositions.size()); i++)
	{
		GraphicsHelper::InsertSprite(
			spriteIdList[i],
			topRightConnerOfSpritePositions[i].first,
			topRightConnerOfSpritePositions[i].second,
			topRightConnerOfSpritePositions[i].second + TURRET_WIDTH,
			topRightConnerOfSpritePositions[i].first + TURRET_HEIGHT,
			DIRECTION::LEFT,
			textureId
		);
	}
}

void AutoInscreaseSpriteIdLoadAnimations(std::vector<WALL_TURRET_SPRITE_ID> spriteIdList, ANIMATION_ID animationId)
{
	std::vector<std::tuple<SPRITE_ID, DWORD>> listOfSpriteWithRefreshRate;

	for (int i = 0; std::cmp_less(i, spriteIdList.size()); i++)
	{
		listOfSpriteWithRefreshRate.push_back(std::make_tuple(spriteIdList[i], 0));
	}

	GraphicsHelper::InsertAnimation(animationId, 150, listOfSpriteWithRefreshRate);
}

#pragma endregion


void WallTurret::Fire()
{
	if (Enemy::target->isDead)
	{
		return;
	}
}

void WallTurret::Fire(FLOAT angle, FLOAT vx, FLOAT vy, FLOAT ax, FLOAT ay, DIRECTION direction)
{
	if (Enemy::target->isDead)
	{
		return;
	}
	HasWeapons::Fire(this->position.x, this->position.y + this->h / 2, angle, vx, vy, ax, ay, direction);
}