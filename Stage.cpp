#include   "Bill.h"
#include  "Stage.h"
#include "Camera.h"

Stage:: Stage()
{
	this->bill	 = NULL;
	this->camera = NULL;
}

Stage::~Stage()
{
	//Destroy(bill);
	Destroy(camera);
	for (auto& entity : entities) Destroy(entity); entities.clear();
	for (auto& backgroundTerrain : backgroundTerrains) Destroy(backgroundTerrain); backgroundTerrains.clear();
	for (auto& collidableTerrain : collidableTerrains) Destroy(collidableTerrain); collidableTerrains.clear();
}

void  Stage::SetBill(Bill* bill) {		  this->bill = bill; bill = NULL; }
Bill* Stage::GetBill(          ) { return this->bill       ;              }

void    Stage::SetCamera(Camera* camera) {		  this->camera = camera; camera = NULL; }
Camera* Stage::GetCamera(              ) { return this->camera         ;                }
