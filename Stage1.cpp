#include "Bill.h"
#include "Stage1.h"
#include "tileson.hpp"
#include "TerrainStage1.h"
#include "Camera.h"
Stage1::Stage1() : Stage()
{
}

Stage1::~Stage1()
{
}

void Stage1::Load()
{
	LoadMap();
}

void Stage1::Update()
{
}

void Stage1::Render()
{
	for (auto& backgroundTerrain : backgroundTerrains) 
		if (backgroundTerrain->GetX() <= _cam->GetX() + (SCREEN_WIDTH / SCALING_RATIO_X / 2.0f) + 16
			&& backgroundTerrain->GetX() >= _cam->GetX() - (SCREEN_WIDTH / SCALING_RATIO_X / 2.0f) - 16) backgroundTerrain->Render();
}

void Stage1::HandleInput(Input&)
{
}

void Stage1::LoadMap()
{
	tson::Tileson tileson; std::unique_ptr<tson::Map> map = tileson.parse(fs::path("Resources/Maps/stage1.json"));
	tson::Layer* backgroundTerrainsLayer = map.get()->getLayer("BackgroundLayer");
	tson::Layer* collidableTerrainsLayer = map.get()->getLayer("CollidableLayer");
	tson::Layer* entitiesLayer = map.get()->getLayer("ObjectLayer");
	LoadBackgroundTerrains(backgroundTerrainsLayer);
	LoadCollidableTerrains(collidableTerrainsLayer);
	LoadEntities(entitiesLayer);
}

void Stage1::LoadEntities(void* entitiesLayer)
{
	auto _entitiesLayer = (tson::Layer*)entitiesLayer;
}

void Stage1::LoadBackgroundTerrains(void* backgroundTerrainsLayer)
{
	auto _backgroundTerrainsLayer = (tson::Layer*)backgroundTerrainsLayer;
	for (auto& [tileObjectPosition, tileObject] : _backgroundTerrainsLayer->getTileObjects()) // tileObjectPosition = (row number, column number)
	{
		TerrainStage1* backgroundTerrain = new TerrainStage1(); auto& position = tileObject.getPosition();
		auto& size = tileObject.getTile()->getTileSize(); auto animationId = std::to_string(tileObject.getTile()->getId());
		backgroundTerrain->SetAnimationId(animationId);
		backgroundTerrain->SetX(position.x + size.x * 0.5f);
		backgroundTerrain->SetY(position.y + size.y * 0.0f);
		backgroundTerrains.push_back(backgroundTerrain);
	}
	std::vector<Entity*> temp{ std::make_move_iterator(std::begin(backgroundTerrains)), std::make_move_iterator(std::end(backgroundTerrains)) };
	backgroundTerrains.clear();
	for (int i = 0; i < temp.size() / 2; i++)
	{
		auto y1 = temp[                  i]->GetY();
		auto y2 = temp[temp.size() - 1 - i]->GetY();
		std::swap(y1, y2);
		temp[                  i]->SetY(y1);
		temp[temp.size() - 1 - i]->SetY(y2);
		backgroundTerrains.assign(temp.begin(), temp.end());
	}
	backgroundTerrains.sort([](Entity* e1, Entity* e2) -> bool { return e1->GetX() < e2->GetX(); });
}

void Stage1::LoadCollidableTerrains(void* collidableTerrainsLayer)
{
	auto _collidableTerrainsLayer = (tson::Layer*)collidableTerrainsLayer;
}

void Stage1::RenderMap()
{
}

void Stage1::RenderEntities()
{
}

void Stage1::RenderBackgroundTerrains()
{
}

void Stage1::RenderCollidableTerrains()
{
}
