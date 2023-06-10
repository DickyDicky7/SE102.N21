#include "Bill.h"
#include "Stage1.h"
#include "Soldier.h"
#include "tileson.hpp"
#include "WallTurret.h"
#include "RifleManStanding.h"
#include "RifleManHideOnBush.h"
#include "AirCraft.h"
#include "Falcon.h"
#include "Cannon.h"
#include "Explosion.h"
#include "Item.h"

Stage1:: Stage1() : Stage()
{
	mapFilePath = "Resources/Maps/stage1.json";
}

Stage1::~Stage1()
{
}

void Stage1::LoadEntities(void* entitiesLayer)
{
	auto _entitiesLayer = (tson::Layer*)entitiesLayer;
	auto mapH = _entitiesLayer->getMap()->getSize    ().y
              * _entitiesLayer->getMap()->getTileSize().y;

	for (auto& object : _entitiesLayer->getObjects())
	{
		auto& position = object.getPosition(); auto& size = object.getSize();
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
			entity = new Cannon();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "soldierl")
		{
			//entity = new Soldier();
			entity = new Item(ITEM_TYPE::M);
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
			entity = new Falcon(ITEM_TYPE::M);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "staticweaponf")
		{
			entity = new Falcon(ITEM_TYPE::F);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "staticweapons")
		{
			entity = new Falcon(ITEM_TYPE::S);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "capsuleweaponr")
		{
			entity = new AirCraft(ITEM_TYPE::R, AIRCRAFT_DIRECTION::HORIZONTAL);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else
		if (object.getName() == "capsuleweaponl")
		{
			entity = new AirCraft(ITEM_TYPE::L, AIRCRAFT_DIRECTION::HORIZONTAL);
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
		if (enemy) enemy->SetTarget(bill);

		entity->SetX(       position.x + size.x * 0.5f);
		entity->SetY(mapH - position.y - size.y * 1.0f);
		entity->SetW(FLOAT(size.x));
		entity->SetH(FLOAT(size.y));

		entities->Insert(entity);
	}

	auto representativeBill = new Bill();
	auto representativeBullet = new Bullet();
	auto representativeFalcon = new Falcon(ITEM_TYPE::I);
	auto representativeSoldier = new Soldier();
	auto representativeAirCraft = new AirCraft(ITEM_TYPE::I, AIRCRAFT_DIRECTION::HORIZONTAL);
	auto representativeWallTurret = new WallTurret();
	auto representativeRifleManStanding = new RifleManStanding();
	auto representativeRifleManHideOnBush = new RifleManHideOnBush();
	auto representativeCannon = new Cannon();

	representativeBill->LoadTextures();
	representativeFalcon->LoadTextures();
	representativeBullet->LoadTextures();
	representativeSoldier->LoadTextures();
	representativeAirCraft->LoadTextures();
	representativeWallTurret->LoadTextures();
	representativeRifleManStanding->LoadTextures();
	representativeRifleManHideOnBush->LoadTextures();
	representativeCannon->LoadTextures();

	representativeBill->LoadSprites();
	representativeFalcon->LoadSprites();
	representativeBullet->LoadSprites();
	representativeSoldier->LoadSprites();
	representativeAirCraft->LoadSprites();
	representativeWallTurret->LoadSprites();
	representativeRifleManStanding->LoadSprites();
	representativeRifleManHideOnBush->LoadSprites();
	representativeCannon->LoadSprites();

	representativeBill->LoadAnimations();
	representativeFalcon->LoadAnimations();
	representativeBullet->LoadAnimations();
	representativeSoldier->LoadAnimations();
	representativeAirCraft->LoadAnimations();
	representativeWallTurret->LoadAnimations();
	representativeRifleManStanding->LoadAnimations();
	representativeRifleManHideOnBush->LoadAnimations();
	representativeCannon->LoadAnimations();

	Destroy(representativeBill);
	Destroy(representativeFalcon);
	Destroy(representativeBullet);
	Destroy(representativeSoldier);
	Destroy(representativeAirCraft);
	Destroy(representativeWallTurret);
	Destroy(representativeRifleManStanding);
	Destroy(representativeRifleManHideOnBush);
	Destroy(representativeCannon);

	// explosion
	auto representativeExplosion = new Explosion();
	representativeExplosion->LoadTextures();
	representativeExplosion->LoadSprites();
	representativeExplosion->LoadAnimations();
	Destroy(representativeExplosion);

	// item
	auto representativeItem = new Item(ITEM_TYPE::I);
	representativeItem->LoadTextures();
	representativeItem->LoadSprites();
	representativeItem->LoadAnimations();
	Destroy(representativeItem);
}
