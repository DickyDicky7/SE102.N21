#include "Bill.h" 
#include "Stage.h"
#include "Camera.h"
//#include "QuadTreeContainer.h"

Stage::Stage() : bill(NULL), tileW(0.0f), tileH(0.0f), camera(NULL),/* _quadTreeContainer(NULL)*/ _quadTreeNodeE(NULL), _quadTreeNodeB(NULL)
{
}

Stage::~Stage()
{
	Destroy(bill); Destroy(camera); /*Destroy(_quadTreeContainer);*/ Destroy(_quadTreeNodeE); Destroy(_quadTreeNodeB);
	for (auto& entity : entities) Destroy(entity); entities.clear();
	for (auto& backgroundTerrain : backgroundTerrains) Destroy(backgroundTerrain); backgroundTerrains.clear();
	for (auto& foregroundTerrain : foregroundTerrains) Destroy(foregroundTerrain); foregroundTerrains.clear();
}

void Stage::Update()
{
	bill->Update();
	//for (auto& entity : entities)
	//if  (IsLORPointOnTheScreenByX(entity) 
	//&&   IsTOBPointOnTheScreenByY(entity)) entity->Update();
}
#include "QuadTreeNode.h"
void Stage::Render()
{
	//for (auto& backgroundTerrain : backgroundTerrains)
	//if  (IsLORPointOnTheScreenByX(backgroundTerrain)
	//&&   IsTOBPointOnTheScreenByY(backgroundTerrain)) backgroundTerrain->Render();

	std::unordered_set<Entity*> ss;
	_quadTreeNodeB->Retrieve(camera, ss);
	for (auto& bg : ss)
		bg->Render();

	//for (auto& entity : entities)
	//if  (IsLORPointOnTheScreenByX(entity)
	//&&   IsTOBPointOnTheScreenByY(entity)) entity->Render();

	std::unordered_set<Entity*> s;
	_quadTreeNodeE->Retrieve(camera, s);
	//_RPT1(0, "%d\n", s.size());
	for (auto& e : s)
		e->Render();

	bill->Render();
}

void Stage::HandleInput(Input& input)
{
	bill->HandleInput(input);
}

void Stage::CheckResolveClearCollision()
{	
	//for (auto it = _quadTreeContainer->begin(); it != _quadTreeContainer->end(); it++)
	//			   _quadTreeContainer->Relocate(it);

	//std::list<Entity*> couldBeCollidedWithEntities = _quadTreeContainer->GetCollisionWithTarget(bill);
	//for (auto& couldBeCollidedWithEntity : couldBeCollidedWithEntities)
	//	   if (couldBeCollidedWithEntity != bill) bill->CollideWith(couldBeCollidedWithEntity);

	//_RPT1(0, "%d\n", result.size());
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
