#include <vector>
#include "WallTurret.h"

int TurretHeight, TurretWidth = TurretHeight = 32;

void AutoIncreasePositionSpriteLoader(
	std::vector<std::pair<int, int>> TopRightConnerOfSpritePositions,
	SPRITE_ID FirstSpriteId,
	TEXTURE_ID TextureId
);
void AutoInscreaseSpriteIdLoadAnimations(SPRITE_ID FirstSpriteId, int NumberOfSprites, ANIMATION_ID AnimationId);

WallTurret::WallTurret() : Entity(), HasAnimations()
{
	self = this;

	this->vx = 1;
	this->vy = 1;
	this->ax = 0.1;
	this->ay = 0.1;
	this->position.x = SCREEN_WIDTH / 3;
	this->position.y = SCREEN_HEIGHT / 2 - 100;

	this->updateState = NULL;
	this->state = new WallTurretUpState();
}

WallTurret::~WallTurret() {}

void WallTurret::Update() {
	updateState = state->Update(*this);
}

void WallTurret::Render() {
	state->Render(*this);
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


void WallTurret::LoadTextures() {
	if (HasTextures<WallTurret>::hasBeenLoaded.value) {
		return;
	}
	HasTextures<WallTurret>::hasBeenLoaded.value = true;

	//GraphicsDatabase::textures.insert({ WALL_TURRET, GraphicsHelper::CreateTexture(L"Resources\\Textures\\Wall_turret.bmp") });

	return;
}

void WallTurret::LoadSprites() {

	if (HasSprites<WallTurret>::hasBeenLoaded.value) {
		return;
	}
	HasSprites<WallTurret>::hasBeenLoaded.value = true;

//	std::vector<std::pair<int, int>> TopRightConnerOfSpritePositions;
//
//#pragma region WALL_TURRET_LEFT SPRITE
//
//	TopRightConnerOfSpritePositions.push_back({ 69, 1 });
//	TopRightConnerOfSpritePositions.push_back({ 69, 35 });
//	TopRightConnerOfSpritePositions.push_back({ 69, 69 });
//
//	AutoIncreasePositionSpriteLoader(TopRightConnerOfSpritePositions, WALL_TURRET_LEFT_01, WALL_TURRET);
//
//	TopRightConnerOfSpritePositions.clear();
//
//#pragma endregion
//
//#pragma region WALL_TURRET_RIGHT SPRITE
//
//	TopRightConnerOfSpritePositions.push_back({ 1, 1 });
//	TopRightConnerOfSpritePositions.push_back({ 1, 35 });
//	TopRightConnerOfSpritePositions.push_back({ 1, 69 });
//
//	AutoIncreasePositionSpriteLoader(TopRightConnerOfSpritePositions, WALL_TURRET_RIGHT_01,WALL_TURRET);
//
//	TopRightConnerOfSpritePositions.clear();
//
//#pragma endregion
//
//#pragma region WALL_TURRET_UP SPRITE
//
//	TopRightConnerOfSpritePositions.push_back({ 103, 1 });
//	TopRightConnerOfSpritePositions.push_back({ 103, 35 });
//	TopRightConnerOfSpritePositions.push_back({ 103, 69 });
//
//	AutoIncreasePositionSpriteLoader(TopRightConnerOfSpritePositions, WALL_TURRET_UP_01, WALL_TURRET);
//
//	TopRightConnerOfSpritePositions.clear();
//
//#pragma endregion
//
//#pragma region WALL_TURRET_DOWN SPRITE
//
//
//	TopRightConnerOfSpritePositions.push_back({ 35, 1 });
//	TopRightConnerOfSpritePositions.push_back({ 35, 35 });
//	TopRightConnerOfSpritePositions.push_back({ 35, 6 });
//
//	AutoIncreasePositionSpriteLoader(TopRightConnerOfSpritePositions, WALL_TURRET_DOWN_01, WALL_TURRET);
//
//	TopRightConnerOfSpritePositions.clear();
//
//#pragma endregion

}

void WallTurret::LoadAnimations() {

	if (HasAnimations<WallTurret>::hasBeenLoaded.value) {
		return;
	}
	HasAnimations<WallTurret>::hasBeenLoaded.value = true;

	//AutoInscreaseSpriteIdLoadAnimations(WALL_TURRET_LEFT_01, 3, WALL_TURRET_LEFT);
	//AutoInscreaseSpriteIdLoadAnimations(WALL_TURRET_RIGHT_01, 3, WALL_TURRET_RIGHT);
	//AutoInscreaseSpriteIdLoadAnimations(WALL_TURRET_DOWN_01, 3, WALL_TURRET_DOWN);
	//AutoInscreaseSpriteIdLoadAnimations(WALL_TURRET_UP_01, 3, WALL_TURRET_UP);
}


#pragma region Helpers

void AutoIncreasePositionSpriteLoader(
	std::vector<std::pair<int, int>> TopRightConnerOfSpritePositions,
	SPRITE_ID FirstSpriteId,
	TEXTURE_ID TextureId
)
{
	for (int i = 0; i < TopRightConnerOfSpritePositions.size(); i++)
	{
		GraphicsDatabase::sprites.insert(
			{ static_cast<SPRITE_ID>(FirstSpriteId + i), GraphicsHelper::CreateSprite(
				TopRightConnerOfSpritePositions[i].first ,
				TopRightConnerOfSpritePositions[i].second ,
				TopRightConnerOfSpritePositions[i].second + TurretWidth ,
				TopRightConnerOfSpritePositions[i].first + TurretHeight ,
				TextureId)
			});
	}
}

void AutoInscreaseSpriteIdLoadAnimations(SPRITE_ID FirstSpriteId, int NumberOfSprites, ANIMATION_ID AnimationId)
{
	std::vector<std::pair<SPRITE_ID, DWORD>> ListOfSpriteWithRefreshRate;

	for (int i = 0; i < NumberOfSprites; i++)
	{
		ListOfSpriteWithRefreshRate.push_back({ static_cast<SPRITE_ID>(FirstSpriteId + i), 0 });
	}

	GraphicsDatabase::animations.insert({ AnimationId, GraphicsHelper::CreateAnimation(150, ListOfSpriteWithRefreshRate) });
}

#pragma endregion