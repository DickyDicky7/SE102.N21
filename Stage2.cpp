#include "Bill.h"
#include "Enemy.h"
#include "Stage2.h"
#include "tileson.hpp"
#include "Fire.h"
#include "Soldier.h"
#include "WallTurret.h"
#include "RifleManStanding.h"
#include "RifleManHideOnBush.h"
#include "AirCraft.h"
#include "Falcon.h"
#include "ScubaSoldier.h"
#include "RockFall.h"
#include "RockFly.h"

Stage2:: Stage2() : Stage()
{
	mapFilePath = "Resources/Maps/stage2.json";
}

Stage2::~Stage2()
{
}

void Stage2::LoadEntities(void* entitiesLayer)
{
	auto _entitiesLayer = (tson::Layer*)entitiesLayer;
	auto  mapH = _entitiesLayer->getMap()->getSize    ().y 
	           * _entitiesLayer->getMap()->getTileSize().y;

	for (auto& object : _entitiesLayer->getObjects())
	{
		auto& position = object.getPosition();
		auto& size     = object.getSize();
		Entity* entity = NULL;
		
		if (object.getName() == "blazerL")
		{
			entity = new Fire();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "blazerR")
		{
			entity = new Fire();
			entity->SetMovingDirection(DIRECTION::RIGHT);
		}
		else if (object.getName() == "soldiershotr")
		{
			entity = new Soldier();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "soldiershotf")
		{
			entity = new Soldier();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "soldiershotl")
		{
			entity = new Soldier();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "soldiershotr")
		{
			entity = new Soldier();
			entity->SetMovingDirection(DIRECTION::LEFT);
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
			//entity = new RifleManHideOnBush();
			//entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "snipere")
		{
			//entity = new RifleManStanding();
			//entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "failingstone")
		{
			entity = new RockFall();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "dynamicbridge1")
		{
			entity = new RockFly(80, 176);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "dynamicbridge2")
		{
			entity = new RockFly(64, 144);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "dynamicbridge3")
		{
			entity = new RockFly(80, 224);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "gunrotating3")
		{
			entity = new WallTurret();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "staticweaponm")
		{
			entity = new Falcon(AMMO_TYPE::M);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "staticweaponf")
		{
			entity = new Falcon(AMMO_TYPE::F);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "staticweaponl")
		{
			entity = new Falcon(AMMO_TYPE::L);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "staticweapons")
		{
			entity = new Falcon(AMMO_TYPE::S);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "capsuleweapon3b")
		{
			entity = new AirCraft(AMMO_TYPE::B, AIRCRAFT_DIRECTION::VERTICAL);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "capsuleweapon3r")
		{
			entity = new AirCraft(AMMO_TYPE::R, AIRCRAFT_DIRECTION::VERTICAL);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else 
		if (object.getName() == "respawnposition")
		{
			bill->SetX(       position.x + size.x * 0.5f);
			bill->SetY(mapH - position.y - size.y * 1.0f);
		}
		else 
		if (object.getName() == "cameratranslateposition")
		{

		}

		if (!entity) continue;

		auto enemy = dynamic_cast<Enemy<Bill>*>(entity); 
		if  (enemy) enemy->SetTarget(bill);

		entity->SetX(		position.x + size.x * 0.5f);
		entity->SetY(mapH - position.y - size.y * 1.0f);
		entity->SetW(FLOAT(size.x));
		entity->SetH(FLOAT(size.y));

		entities->Insert(entity);
	}

	// fire
	auto representativeFire = new Fire();
	representativeFire->LoadTextures();
	representativeFire->LoadSprites();
	representativeFire->LoadAnimations();
	Destroy(representativeFire);

	// static weaponm
	auto representativeFalcon = new Falcon(AMMO_TYPE::I);
	representativeFalcon->LoadTextures();
	representativeFalcon->LoadSprites();
	representativeFalcon->LoadAnimations();
	Destroy(representativeFalcon);

	// aircraft
	auto representativeAirCraft = new AirCraft(AMMO_TYPE::I, AIRCRAFT_DIRECTION::VERTICAL);
	representativeAirCraft->LoadTextures();
	representativeAirCraft->LoadSprites();
	representativeAirCraft->LoadAnimations();
	Destroy(representativeAirCraft);

	// rockfall
	auto representativeRockFall = new RockFall();
	representativeRockFall->LoadTextures();
	representativeRockFall->LoadSprites();
	representativeRockFall->LoadAnimations();
	Destroy(representativeRockFall);

	// rockflay
	auto representativeRockFLy = new RockFly(0,0);
	representativeRockFLy->LoadTextures();
	representativeRockFLy->LoadSprites();
	representativeRockFLy->LoadAnimations();
	Destroy(representativeRockFLy);

	auto representativeBill = new Bill();
	auto representativeBullet = new Bullet();
	auto representativeSoldier = new Soldier();
	auto representativeWallTurret = new WallTurret();
	auto representativeRifleManStanding = new RifleManStanding();
	//auto representativeRifleManHideOnBush = new RifleManHideOnBush();

	representativeBill->LoadTextures();
	representativeBullet->LoadTextures();
	representativeSoldier->LoadTextures();
	representativeWallTurret->LoadTextures();
	representativeRifleManStanding->LoadTextures();
	//representativeRifleManHideOnBush->LoadTextures();

	representativeBill->LoadSprites();
	representativeBullet->LoadSprites();
	representativeSoldier->LoadSprites();
	representativeWallTurret->LoadSprites();
	representativeRifleManStanding->LoadSprites();
	//representativeRifleManHideOnBush->LoadSprites();

	representativeBill->LoadAnimations();
	representativeBullet->LoadAnimations();
	representativeSoldier->LoadAnimations();
	representativeWallTurret->LoadAnimations();
	representativeRifleManStanding->LoadAnimations();
	//representativeRifleManHideOnBush->LoadAnimations();

	Destroy(representativeBill);
	Destroy(representativeBullet);
	Destroy(representativeSoldier);
	Destroy(representativeWallTurret);
	Destroy(representativeRifleManStanding);
	//Destroy(representativeRifleManHideOnBush);
}

