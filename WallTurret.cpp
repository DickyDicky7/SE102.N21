#include "WallTurret.h"

int TurretHeight, TurretWidth = TurretHeight = 32;

void AutoIncreasePositionSpriteLoader(
	std::vector<std::pair<int, int>> TopRightConnerOfSpritePositions,
	std::vector<std::pair<SPRITE_ID, DWORD>> SpriteIdList,
	ANIMATION_ID AnimationId,
	TEXTURE_ID TextureId
);

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


void WallTurret::LoadSprite() {
	GraphicsDatabase::textures.insert({ WALL_TURRET, GraphicsHelper::CreateTexture(L"Resources\\Textures\\Wall_turret.bmp") });

	std::vector<std::pair<int, int>> TopRightConnerOfSpritePositions;
	std::vector<std::pair<SPRITE_ID, DWORD>> SpriteIdList;

#pragma region WALL_TURRET_LEFT SPRITE

	TopRightConnerOfSpritePositions.push_back({ 69, 1 });
	TopRightConnerOfSpritePositions.push_back({ 69, 35 });
	TopRightConnerOfSpritePositions.push_back({ 69, 69 });

	SpriteIdList.push_back({ WALL_TURRET_LEFT_01, 0 });
	SpriteIdList.push_back({ WALL_TURRET_LEFT_02, 0 });
	SpriteIdList.push_back({ WALL_TURRET_LEFT_03, 0 });

	AutoIncreasePositionSpriteLoader(TopRightConnerOfSpritePositions, SpriteIdList, WALL_TURRET_LEFT, WALL_TURRET);

	TopRightConnerOfSpritePositions.clear();
	SpriteIdList.clear();

#pragma endregion

#pragma region WALL_TURRET_RIGHT SPRITE

	TopRightConnerOfSpritePositions.push_back({ 1, 1 });
	TopRightConnerOfSpritePositions.push_back({ 1, 35 });
	TopRightConnerOfSpritePositions.push_back({ 1, 69 });

	SpriteIdList.push_back({ WALL_TURRET_RIGHT_01, 0 });
	SpriteIdList.push_back({ WALL_TURRET_RIGHT_02, 0 });
	SpriteIdList.push_back({ WALL_TURRET_RIGHT_03, 0 });

	AutoIncreasePositionSpriteLoader(TopRightConnerOfSpritePositions, SpriteIdList, WALL_TURRET_RIGHT, WALL_TURRET);

	TopRightConnerOfSpritePositions.clear();
	SpriteIdList.clear();

#pragma endregion

#pragma region WALL_TURRET_UP SPRITE

	TopRightConnerOfSpritePositions.push_back({ 103, 1 });
	TopRightConnerOfSpritePositions.push_back({ 103, 35 });
	TopRightConnerOfSpritePositions.push_back({ 103, 69 });

	SpriteIdList.push_back({ WALL_TURRET_UP_01, 0 });
	SpriteIdList.push_back({ WALL_TURRET_UP_02, 0 });
	SpriteIdList.push_back({ WALL_TURRET_UP_03, 0 });

	AutoIncreasePositionSpriteLoader(TopRightConnerOfSpritePositions, SpriteIdList, WALL_TURRET_UP, WALL_TURRET);

	TopRightConnerOfSpritePositions.clear();
	SpriteIdList.clear();

#pragma endregion

#pragma region WALL_TURRET_DOWN SPRITE


	TopRightConnerOfSpritePositions.push_back({ 35, 1 });
	TopRightConnerOfSpritePositions.push_back({ 35, 35 });
	TopRightConnerOfSpritePositions.push_back({ 35, 6 });

	SpriteIdList.push_back({ WALL_TURRET_DOWN_01, 0 });
	SpriteIdList.push_back({ WALL_TURRET_DOWN_01, 0 });
	SpriteIdList.push_back({ WALL_TURRET_DOWN_01, 0 });

	AutoIncreasePositionSpriteLoader(TopRightConnerOfSpritePositions, SpriteIdList, WALL_TURRET_DOWN, WALL_TURRET);

	TopRightConnerOfSpritePositions.clear();
	SpriteIdList.clear();

#pragma endregion

}

void AutoIncreasePositionSpriteLoader(
	std::vector<std::pair<int, int>> TopRightConnerOfSpritePositions,
	std::vector<std::pair<SPRITE_ID, DWORD>> SpriteIdList,
	ANIMATION_ID AnimationId,
	TEXTURE_ID TextureId
)
{
	std::vector<std::pair<SPRITE_ID, DWORD>> SpriteIdCombineWithFrameRefresh;

	for (int i = 0; i < TopRightConnerOfSpritePositions.size(); i++)
	{
		GraphicsDatabase::sprites.insert(
			{ SpriteIdList[i].first, GraphicsHelper::CreateSprite(
				TopRightConnerOfSpritePositions[i].first ,
				TopRightConnerOfSpritePositions[i].second ,
				TopRightConnerOfSpritePositions[i].second + TurretWidth ,
				TopRightConnerOfSpritePositions[i].first + TurretHeight ,
				TextureId)
			});
		SpriteIdCombineWithFrameRefresh.push_back(SpriteIdList[i]);
	}

	GraphicsDatabase::animations.insert({ AnimationId, GraphicsHelper::CreateAnimation(150, SpriteIdCombineWithFrameRefresh) });
}