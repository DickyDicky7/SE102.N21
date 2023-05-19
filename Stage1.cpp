#include "Bill.h"
#include "Stage1.h"
#include "Camera.h"
#include "Soldier.h"
#include "tileson.hpp"
#include "WallTurret.h"
#include "QuadTreeNode.h"
#include "TerrainBlock.h"
#include "TerrainStage1.h"
#include "RifleManStanding.h"
#include "RifleManHideOnBush.h"

Stage1::Stage1() : Stage()
{
}

Stage1::~Stage1()
{
}

void Stage1::Load()
{
	tson::Tileson tileson; std::unique_ptr<tson::Map> map = tileson.parse(fs::path("Resources/Maps/stage1.json"));
	tson::Layer* backgroundTerrainsLayer = map.get()->getLayer("BackgroundLayer");
	tson::Layer* foregroundTerrainsLayer = map.get()->getLayer("ForegroundLayer");
	tson::Layer* entitiesLayer = map.get()->getLayer("ObjectLayer");
	auto& tileSize = map.get()->getTileSize();
	tileW = tileSize.x;
	tileH = tileSize.y;
	auto& mapSize = map.get()->getSize();
	entities            = QuadTreeNode::New(0.0f, 0.0f, mapSize.x * tileSize.x, mapSize.y * tileSize.y);
	backgroundTerrains  = QuadTreeNode::New(0.0f, 0.0f, mapSize.x * tileSize.x, mapSize.y * tileSize.y);
	foregroundTerrains  = QuadTreeNode::New(0.0f, 0.0f, mapSize.x * tileSize.x, mapSize.y * tileSize.y);
	if (!  bill)   bill = new Bill();
	if (!camera) camera = new Camera(new CameraMovingForwardState());
	LoadBackgroundTerrains(backgroundTerrainsLayer);
	LoadForegroundTerrains(foregroundTerrainsLayer);
	LoadEntities(entitiesLayer);
	entities->Insert(bill);
}

void Stage1::LoadEntities(void* entitiesLayer)
{
	auto _entitiesLayer = (tson::Layer*)entitiesLayer;
	for (auto& object : _entitiesLayer->getObjects())
	{
		Entity* entity = NULL;
		if (object.getName() == "bridge")
		{
			
		}
		else 
		if (object.getName() == "sniper")
		{
			entity = new RifleManStanding();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "sniperh")
		{
			entity = new RifleManHideOnBush();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "snipere")
		{
			entity = new RifleManStanding();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "cannon1")
		{

		}
		else
		if (object.getName() == "soldierl")
		{
			entity = new Soldier();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "gunboss1")
		{

		}
		else 
		if (object.getName() == "finalboss1")
		{

		}
		else 
		if (object.getName() == "gunrotating1")
		{
			entity = new WallTurret();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "staticweaponm")
		{

		}
		else 
		if (object.getName() == "staticweaponf")
		{

		}
		else 
		if (object.getName() == "staticweapons")
		{

		}
		else 
		if (object.getName() == "capsuleweaponr")
		{

		}
		else 
		if (object.getName() == "capsuleweaponl")
		{

		}
		else 
		if (object.getName() == "respawnposition")
		{

		}
		else 
		if (object.getName() == "cameratranslateposition")
		{

		}
		if (!entity) continue;
		auto enemy = dynamic_cast<Enemy<Bill>*>(entity); 
		if  (enemy) enemy->SetTarget(bill);
		auto  mapH = _entitiesLayer->getMap()->getSize().y * _entitiesLayer->getMap()->getTileSize().y;
		auto& position = object.getPosition();
		auto& size = object.getSize();
		entity->SetX(		position.x + size.x * 0.5f);
		entity->SetY(mapH - position.y - size.y * 1.0f);
		entity->SetW(size.x);
		entity->SetH(size.y);
		entities->Insert(entity);
	}

	auto representativeBill = new Bill();
	auto representativeBullet = new Bullet();
	auto representativeSoldier = new Soldier();
	auto representativeWallTurret = new WallTurret();
	auto representativeRifleManStanding = new RifleManStanding();
	auto representativeRifleManHideOnBush = new RifleManHideOnBush();

	representativeBill->LoadTextures();
	representativeBullet->LoadTextures();
	representativeSoldier->LoadTextures();
	representativeWallTurret->LoadTextures();
	representativeRifleManStanding->LoadTextures();
	representativeRifleManHideOnBush->LoadTextures();

	representativeBill->LoadSprites();
	representativeBullet->LoadSprites();
	representativeSoldier->LoadSprites();
	representativeWallTurret->LoadSprites();
	representativeRifleManStanding->LoadSprites();
	representativeRifleManHideOnBush->LoadSprites();

	representativeBill->LoadAnimations();
	representativeBullet->LoadAnimations();
	representativeSoldier->LoadAnimations();
	representativeWallTurret->LoadAnimations();
	representativeRifleManStanding->LoadAnimations();
	representativeRifleManHideOnBush->LoadAnimations();

	Destroy(representativeBill);
	Destroy(representativeBullet);
	Destroy(representativeSoldier);
	Destroy(representativeWallTurret);
	Destroy(representativeRifleManStanding);
	Destroy(representativeRifleManHideOnBush);
}

void Stage1::LoadBackgroundTerrains(void* backgroundTerrainsLayer)
{
	auto _backgroundTerrainsLayer = (tson::Layer*)backgroundTerrainsLayer; std::list<Entity*> _backgroundTerrains;
	for (auto& [tileObjectPosition, tileObject] : _backgroundTerrainsLayer->getTileObjects()) // tileObjectPosition = (row number, column number)
	{
		TerrainStage1* _backgroundTerrain = new TerrainStage1(); auto animationId = std::to_string(tileObject.getTile()->getId());
		auto& position = tileObject.getPosition(); auto& size = tileObject.getTile()->getTileSize();
		_backgroundTerrain->SetW(size.x);
		_backgroundTerrain->SetH(size.y);
		_backgroundTerrain->SetAnimationId(animationId);
		_backgroundTerrain->SetX(position.x + size.x * 0.5f);
		_backgroundTerrain->SetY(position.y + size.y * 0.0f);
		_backgroundTerrains.push_back(_backgroundTerrain);
	}
	std::vector<Entity*> temp{ std::make_move_iterator(std::begin(_backgroundTerrains)), std::make_move_iterator(std::end(_backgroundTerrains)) };
	_backgroundTerrains.clear();
	for (int i = 0; i < temp.size() / 2; i++)
	{
		auto y1 = temp[                  i]->GetY();
		auto y2 = temp[temp.size() - 1 - i]->GetY();
		std::swap(y1, y2);
		temp[                  i]->SetY(y1);
		temp[temp.size() - 1 - i]->SetY(y2);
	}
	_backgroundTerrains.assign(temp.begin(), temp.end());
	_backgroundTerrains.sort([](Entity* e1, Entity* e2) -> bool { return e1->GetX() < e2->GetX(); });
	auto representativeBackgroundTerrain = dynamic_cast<TerrainStage1*>(_backgroundTerrains.front());
		 representativeBackgroundTerrain->LoadTextures  ();
		 representativeBackgroundTerrain->LoadSprites   ();
		 representativeBackgroundTerrain->LoadAnimations();
	for (auto& _backgroundTerrain : _backgroundTerrains)
	 backgroundTerrains->Insert(_backgroundTerrain);
	_backgroundTerrains.clear();
}

void Stage1::LoadForegroundTerrains(void* foregroundTerrainsLayer)
{
	auto _foregroundTerrainsLayer = (tson::Layer*)foregroundTerrainsLayer;
	auto mapH = _foregroundTerrainsLayer->getMap()->getSize().y * _foregroundTerrainsLayer->getMap()->getTileSize().y;
	for (auto& object : _foregroundTerrainsLayer->getObjects())
	{
		TerrainBlock* foregroundTerrain = new TerrainBlock();
		auto& position = object.getPosition();
		auto& size = object.getSize();
		foregroundTerrain->SetX(	   position.x + size.x * 0.5f);
		foregroundTerrain->SetY(mapH - position.y - size.y * 1.0f);
		foregroundTerrain->SetW(size.x);
		foregroundTerrain->SetH(size.y);
		if (object.getClassType() == "throughable")
		foregroundTerrain->type = TERRAIN_BLOCK_TYPE::THROUGHABLE;
		if (object.getClassType() == "water")
		foregroundTerrain->type = TERRAIN_BLOCK_TYPE::WATER;
		if (object.getClassType() == "")
		foregroundTerrain->type = TERRAIN_BLOCK_TYPE::_;
		foregroundTerrains->Insert(foregroundTerrain);
	}
}