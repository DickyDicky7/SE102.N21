#include "Bill.h" 
#include "Stage.h"
#include "Camera.h"
#include "tileson.hpp"
#include "TerrainBlock.h"
#include "TerrainStage1.h"
#include "TerrainStage2.h"


Stage:: Stage() : mapFilePath(""), bill(NULL), tileW(0.0f), tileH(0.0f), camera(NULL), entities(NULL), backgroundTerrains(NULL), foregroundTerrains(NULL)
{
}


Stage::~Stage()
{
	entities->Clean();
	backgroundTerrains->Clean();
	foregroundTerrains->Clean();
	/*Destroy(bill), */Destroy(camera); Destroy(entities); Destroy(backgroundTerrains), Destroy(foregroundTerrains);
}


void Stage::Update()
{
	auto& bullets = HasWeapons::GetBullets();
	for (auto& bullet : bullets)
	{
		if (camera->CouldSee(bullet))
			entities->Insert(bullet); else Destroy(bullet);
	}
	bullets.clear();

	entitiesResult.clear();
	entities->Retrieve(camera,  entitiesResult);
	for (auto& [entity, node] : entitiesResult) entity->Update();
	//for (auto& [e, n] : entitiesResult) if (!camera->CouldSee(e)) OutputDebugString(L"YES\n");

	std::list<Entity*> outOfBoundBullets;
	for (auto& [entity, node] : entitiesResult)
	{
		if (dynamic_cast<Bullet*>(entity))
		{
			if (!camera->CouldSee(entity))
			{
				node->entities.remove(entity);
				outOfBoundBullets.push_back(entity);
			}
		}
	}
	for (auto& outOfBoundBullet : outOfBoundBullets)
	{
		entitiesResult.erase(outOfBoundBullet);
		Destroy(outOfBoundBullet);
		//OutputDebugString(L"ERASED\n");
	}

	if (QuadTreeNode::Update(entities, entitiesResult))
	{
		entitiesResult.clear();
		entities->Retrieve(camera, entitiesResult);

		//for (auto& [e, n] : entitiesResult) if (!camera->CouldSee(e)) OutputDebugString(L"YES\n");
	}
	//for (auto& [e, n] : entitiesResult) if (!camera->CouldSee(e)) OutputDebugString(L"YES\n");
}


void Stage::Render()
{
	backgroundTerrainsResult.clear();
	backgroundTerrains->Retrieve(camera,   backgroundTerrainsResult);
	for (auto& [backgroundTerrain, node] : backgroundTerrainsResult) backgroundTerrain->Render();
	for (auto& [entity           , node] : entitiesResult)                      entity->Render();
}


void Stage::HandleInput(Input& input)
{
	bill->HandleInput(input);
}


void Stage::CheckResolveClearCollision()
{
	foregroundTerrainsResult.clear();
	foregroundTerrains->Retrieve(camera, foregroundTerrainsResult);
	for (auto& [entity, node] : entitiesResult)
	{
		auto collidableEntity = dynamic_cast<CollidableEntity*>(entity);
		if  (collidableEntity)
		{
			for (auto& [foregroundTerrain, node] : foregroundTerrainsResult)
			 collidableEntity->CollideWith(foregroundTerrain);
		}
	}
}


void  Stage::SetBill(Bill* bill) {		  this->bill = bill; bill = NULL; }
Bill* Stage::GetBill(          ) { return this->bill       ;              }


void    Stage::SetCamera(Camera* camera) {		  this->camera = camera; camera = NULL; }
Camera* Stage::GetCamera(              ) { return this->camera         ;                }


template void Stage::Load<TerrainStage1, CameraMovingForwardState>();
template void Stage::Load<TerrainStage2, CameraMovingUpwardState >();
template <class T, class S>
void Stage::Load()
{
	tson::Tileson tileson;
	std::unique_ptr<tson::Map> map = tileson.parse(fs::path(mapFilePath));

	tson::Layer* backgroundTerrainsLayer = map.get()->getLayer("BackgroundLayer");
	tson::Layer* foregroundTerrainsLayer = map.get()->getLayer("ForegroundLayer");
	tson::Layer*           entitiesLayer = map.get()->getLayer("ObjectLayer");
	tson::Tileset& tileset = map.get()->getTilesets()[0];
	auto& tileSize = map.get()->getTileSize();
	auto&  mapSize = map.get()->getSize();

	entities            = QuadTreeNode::New(0.0f, 0.0f, FLOAT(mapSize.x * tileSize.x), FLOAT(mapSize.y * tileSize.y));
	backgroundTerrains  = QuadTreeNode::New(0.0f, 0.0f, FLOAT(mapSize.x * tileSize.x), FLOAT(mapSize.y * tileSize.y));
	foregroundTerrains  = QuadTreeNode::New(0.0f, 0.0f, FLOAT(mapSize.x * tileSize.x), FLOAT(mapSize.y * tileSize.y));
	tileW = FLOAT(tileSize.x);
	tileH = FLOAT(tileSize.y);
	T::SetTileset(&tileset);

	if (!camera) camera = new Camera(new S());
	if (!  bill)   bill = new Bill();

	LoadBackgroundTerrains<T>(backgroundTerrainsLayer);
	LoadForegroundTerrains   (foregroundTerrainsLayer);
	LoadEntities(entitiesLayer);
	entities->Insert(bill);
}


template void Stage::LoadBackgroundTerrains<TerrainStage1>(void*);
template void Stage::LoadBackgroundTerrains<TerrainStage2>(void*);
template <class T>
void Stage::LoadBackgroundTerrains(void* backgroundTerrainsLayer)
{
	auto               _backgroundTerrainsLayer = (tson::Layer*)backgroundTerrainsLayer; 
	std::list<Entity*> _backgroundTerrains;

	for (auto& [tileObjectPosition, tileObject] : _backgroundTerrainsLayer->getTileObjects())
	{
		T* _backgroundTerrain = new T();
		auto animationId = std::to_string(tileObject.getTile()->getId()); auto& position = tileObject.getPosition(); auto& size = tileObject.getTile()->getTileSize();

		_backgroundTerrain->SetW(FLOAT(size.x));
		_backgroundTerrain->SetH(FLOAT(size.y));
		_backgroundTerrain->SetAnimationId(animationId);
		_backgroundTerrain->SetX(position.x + size.x * 0.5f);
		_backgroundTerrain->SetY(position.y + size.y * 0.0f);

		_backgroundTerrains.push_back(_backgroundTerrain);
	}

	std::vector<Entity*> temp{ std::make_move_iterator(std::begin(_backgroundTerrains)), std::make_move_iterator(std::end(_backgroundTerrains)) };
	_backgroundTerrains.clear();
	for (int i = 0; std::cmp_less(i, temp.size() / 2); i++)
	{
		auto y1 = temp[                  i]->GetY();
		auto y2 = temp[temp.size() - 1 - i]->GetY();
		std::swap(y1, y2);
		temp[                  i]->SetY(y1);
		temp[temp.size() - 1 - i]->SetY(y2);
	}
	_backgroundTerrains.assign(temp.begin(), temp.end());
	_backgroundTerrains.sort([](Entity* e1, Entity* e2) -> bool { return e1->GetX() < e2->GetX(); });

	auto representativeBackgroundTerrain = dynamic_cast<T*>(_backgroundTerrains.front());
		 representativeBackgroundTerrain->LoadTextures  ();
		 representativeBackgroundTerrain->LoadSprites   ();
		 representativeBackgroundTerrain->LoadAnimations();

	for (auto& _backgroundTerrain : _backgroundTerrains)
	 backgroundTerrains->Insert(    _backgroundTerrain);
	_backgroundTerrains.clear();
}


void Stage::LoadForegroundTerrains(void* foregroundTerrainsLayer)
{
	auto _foregroundTerrainsLayer = (tson::Layer*)foregroundTerrainsLayer;
	auto mapH = _foregroundTerrainsLayer->getMap()->getSize    ().y 
		      * _foregroundTerrainsLayer->getMap()->getTileSize().y;

	for (auto& object : _foregroundTerrainsLayer->getObjects())
	{
		TerrainBlock* foregroundTerrain = new TerrainBlock();
		auto& position = object.getPosition();
		auto& size     = object.getSize    ();

		foregroundTerrain->SetX(	   position.x + size.x * 0.5f);
		foregroundTerrain->SetY(mapH - position.y - size.y * 1.0f);
		foregroundTerrain->SetW(FLOAT(size.x));
		foregroundTerrain->SetH(FLOAT(size.y));

		if (object.getClassType() == "non_throughable")
		foregroundTerrain->type = TERRAIN_BLOCK_TYPE::NON_THROUGHABLE;
		if (object.getClassType() == "throughable")
		foregroundTerrain->type = TERRAIN_BLOCK_TYPE::THROUGHABLE;
		if (object.getClassType() == "water")
		foregroundTerrain->type = TERRAIN_BLOCK_TYPE::WATER;

		foregroundTerrains->Insert(foregroundTerrain);
	}
}

