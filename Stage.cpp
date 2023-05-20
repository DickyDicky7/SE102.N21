#include "Bill.h" 
#include "Stage.h"
#include "Camera.h"
#include "QuadTreeNode.h"

Stage::Stage() : bill(NULL), tileW(0.0f), tileH(0.0f), camera(NULL), entities(NULL), backgroundTerrains(NULL), foregroundTerrains(NULL)
{
}

Stage::~Stage()
{
	entities->Clean();
	backgroundTerrains->Clean();
	foregroundTerrains->Clean();
	Destroy(bill), Destroy(camera); Destroy(entities); Destroy(backgroundTerrains), Destroy(foregroundTerrains);
}

void Stage::Update()
{
	bill->Update();
	_entities.clear();
	entities->Retrieve(camera, _entities);
	for (auto& _entity : _entities) _entity->Update();

	//for (auto& _entity : _entities)
	//{
	//	auto _hasWeaponsEntity = dynamic_cast<HasWeapons*>(_entity);
	//	if (_hasWeaponsEntity)
	//	{
	//		for (auto& bullet : _hasWeaponsEntity->GetBullets())
	//			bullet->Update();
	//	}
	//}

	//<OlD CODE>
	//for (auto& entity : entities)
	//if  (IsLORPointOnTheScreenByX(entity) 
	//&&   IsTOBPointOnTheScreenByY(entity)) entity->Update();
}

void Stage::Render()
{
	//<OlD CODE>
	//for (auto& backgroundTerrain : backgroundTerrains)
	//if  (IsLORPointOnTheScreenByX(backgroundTerrain)
	//&&   IsTOBPointOnTheScreenByY(backgroundTerrain)) backgroundTerrain->Render();

	//<OlD CODE>
	//for (auto& entity : entities)
	//if  (IsLORPointOnTheScreenByX(entity)
	//&&   IsTOBPointOnTheScreenByY(entity)) entity->Render();

	_backgroundTerrains.clear();
	backgroundTerrains->Retrieve(camera, _backgroundTerrains);
	for (auto& _backgroundTerrain : _backgroundTerrains) _backgroundTerrain->Render();
	for (auto& _entity : _entities) _entity->Render();
	bill->Render();
}

void Stage::HandleInput(Input& input)
{
	bill->HandleInput(input);
}

void Stage::CheckResolveClearCollision()
{
	_foregroundTerrains.clear();
	foregroundTerrains->Retrieve(camera, _foregroundTerrains);
	for (auto& _foregroundTerrain : _foregroundTerrains) bill->CollideWith(_foregroundTerrain);
}

BOOL Stage::IsLORPointOnTheScreenByX(Entity* entity)
{
	return entity->GetX() - entity->GetW() / 2.0f <= camera->GetX() + (SCREEN_WIDTH / SCALING_RATIO_X / 2.0f) + tileW
		&& entity->GetX() - entity->GetW() / 2.0f >= camera->GetX() - (SCREEN_WIDTH / SCALING_RATIO_X / 2.0f) - tileW
		|| entity->GetX() + entity->GetW() / 2.0f <= camera->GetX() + (SCREEN_WIDTH / SCALING_RATIO_X / 2.0f) + tileW
		&& entity->GetX() + entity->GetW() / 2.0f >= camera->GetX() - (SCREEN_WIDTH / SCALING_RATIO_X / 2.0f) - tileW
		 ;
}

BOOL Stage::IsTOBPointOnTheScreenByY(Entity* entity)
{
	return entity->GetY()                         <= camera->GetY() + (SCREEN_HEIGHT / SCALING_RATIO_Y / 2.0f) + tileH
		&& entity->GetY()                         >= camera->GetY() - (SCREEN_HEIGHT / SCALING_RATIO_Y / 2.0f) - tileH
		|| entity->GetY() + entity->GetH() / 2.0f <= camera->GetY() + (SCREEN_HEIGHT / SCALING_RATIO_Y / 2.0f) + tileH
		&& entity->GetY() + entity->GetH() / 2.0f >= camera->GetY() - (SCREEN_HEIGHT / SCALING_RATIO_Y / 2.0f) - tileH
		 ;
}

void  Stage::SetBill(Bill* bill) {		  this->bill = bill; bill = NULL; }
Bill* Stage::GetBill(          ) { return this->bill       ;              }

void    Stage::SetCamera(Camera* camera) {		  this->camera = camera; camera = NULL; }
Camera* Stage::GetCamera(              ) { return this->camera         ;                }
