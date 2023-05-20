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
}

void Stage::Render()
{
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
	foregroundTerrains->Retrieve(bill, tileW * 2, tileH * 2, _foregroundTerrains);
	_RPT1(0, "%d\n", _foregroundTerrains.size());
	for (auto& _foregroundTerrain : _foregroundTerrains) bill->CollideWith(_foregroundTerrain);
}

void  Stage::SetBill(Bill* bill) {		  this->bill = bill; bill = NULL; }
Bill* Stage::GetBill(          ) { return this->bill       ;              }

void    Stage::SetCamera(Camera* camera) {		  this->camera = camera; camera = NULL; }
Camera* Stage::GetCamera(              ) { return this->camera         ;                }
