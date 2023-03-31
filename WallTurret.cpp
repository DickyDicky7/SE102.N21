#include "WallTurret.h"

WallTurret::WallTurret() : Entity(), HasAnimations()
{
	self = this;

	this->vx = 1;
	this->vy = 1;
	this->ax = 0.1;
	this->ay = 0.1;
	this->position.x = 16;
	this->position.y = 32;

	this->updateState = NULL;
	this->state = new WallTurretLeftState();
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

	GraphicsDatabase::sprites.insert({ WALL_TURRET_LEFT_01, GraphicsHelper::CreateSprite(69 ,1 ,33 ,101 ,WALL_TURRET) });
	GraphicsDatabase::sprites.insert({ WALL_TURRET_LEFT_02, GraphicsHelper::CreateSprite(69 ,35 ,67 ,101 ,WALL_TURRET) });
	GraphicsDatabase::sprites.insert({ WALL_TURRET_LEFT_03, GraphicsHelper::CreateSprite(69 ,69 ,101 ,101 ,WALL_TURRET) });

	GraphicsDatabase::animations.insert({ WALL_TURRET_LEFT, GraphicsHelper::CreateAnimation
	(150,
		{
			{WALL_TURRET_LEFT_01, 0},
			{WALL_TURRET_LEFT_02, 0},
			{WALL_TURRET_LEFT_03, 0},
		}
	) });
}