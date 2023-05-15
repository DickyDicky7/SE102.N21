#include  "Bill.h"
#include "Stage.h"

Stage::Stage()
{
	this->bill = new Bill();
}

Stage::~Stage()
{
	Destroy(bill);
	for (auto& entity : entities) Destroy(entity); entities.clear();
	for (auto& backgroundTerrain : backgroundTerrains) Destroy(backgroundTerrain); backgroundTerrains.clear();
	for (auto& collidableTerrain : collidableTerrains) Destroy(collidableTerrain); collidableTerrains.clear();
}
