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

WallTurret::WallTurret() : Entity(), HasAnimations()
{
	Enemy::self = this;
	Entity::self = this;

	this->vx = 1.0f;
	this->vy = 1.0f;
	this->ax = 0.1f;
	this->ay = 0.1f;
	this->position.x = 50;
	this->position.y = 50;
	this->billAngle = -90;

	this->movingDirection = DIRECTION::LEFT;

	this->updateState = NULL;
	this->state = new WallTurretLeft30State();
}

WallTurret::~WallTurret() {}

void WallTurret::Update() {

	CalculateBillAngle();

	if (billAngle >= -105 && billAngle < -75)
	{
		this->state = new WallTurretLeft90State();
	}
	else if (billAngle >= -75 && billAngle < -45)
	{
		this->state = new WallTurretLeft120State();
	}
	else if (billAngle >= -45 && billAngle < -15)
	{
		this->state = new WallTurretLeft150State();
	}
	else if (billAngle >= -15 && billAngle < 15)
	{
		this->state = new WallTurretDownState();
	}
	else if (billAngle >= 15 && billAngle < 45)
	{
		this->state = new WallTurretRight150State();
	}
	else if (billAngle >= 45 && billAngle < 75)
	{
		this->state = new WallTurretRight120State();
	}
	else if (billAngle >= 75 && billAngle < 105)
	{
		this->state = new WallTurretRight90State();
	}
	else if (billAngle >= 105 && billAngle < 135)
	{
		this->state = new WallTurretRight30State();
	}
	else if (billAngle >= 135 && billAngle < 165)
	{
		this->state = new WallTurretRight60State();
	}
	else if (billAngle >= 165 || billAngle < -165)
	{
		this->state = new WallTurretUpState();
	}
	else if (billAngle >= -165 && billAngle < -135)
	{
		this->state = new WallTurretLeft30State();
	}
	else if (billAngle >= -135 && billAngle < -105)
	{
		this->state = new WallTurretLeft60State();
	}
	updateState = state->Update(*this);
}

void WallTurret::Render() {
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		state->Exit(*this);
		delete state;
		state = updateState;
		state->Enter(*this);
		updateState = NULL;
	}
}

void WallTurret::HandleInput(Input& input) {}

void WallTurret::CalculateBillAngle()
{
	float dx = +(this->GetPosition().x - Enemy::target->GetPosition().x);
	float dy = -(this->GetPosition().y - Enemy::target->GetPosition().y);

	if (dx > 0 && dy < 0)
		billAngle = -atan(dx / (abs(dy))) * 180 / PI;
	else if (dx < 0 && dy < 0)
		billAngle = atan(abs(dx) / abs(dy)) * 180 / PI;
	else if (dx > 0 && dy > 0)
		billAngle = atan(dx / dy) * 180 / PI - 180;
	else if (dx < 0 && dy > 0)
		billAngle = -atan(abs(dx) / dy) * 180 / PI + 180;
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

#pragma region LOAD APPEAR ANIMATIONS

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
		WALL_TURRET_ANIMATION_ID::APPEAR
			);

#pragma endregion

#pragma region LOAD CLOSE ANIMATIONS

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
		WALL_TURRET_ANIMATION_ID::CLOSE
			);

#pragma endregion
}


#pragma region Helpers

void AutoIncreasePositionSpriteLoader(
	std::vector<std::pair<int, int>> topRightConnerOfSpritePositions,
	std::vector<WALL_TURRET_SPRITE_ID> spriteIdList,
	TEXTURE_ID textureId
)
{
	for (int i = 0; i < topRightConnerOfSpritePositions.size(); i++)
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

	for (int i = 0; i < spriteIdList.size(); i++)
	{
		listOfSpriteWithRefreshRate.push_back(std::make_tuple(spriteIdList[i], 0));
	}

	GraphicsHelper::InsertAnimation(animationId, 150, listOfSpriteWithRefreshRate);
}

#pragma endregion
