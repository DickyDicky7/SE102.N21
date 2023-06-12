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
#include "ScubaSoldier.h"
#include "BossStage3.h"
#include "BossStage3Gate.h"
#include "BossStage3Hand.h"
#include "Explosion.h"
#include "Item.h"
#include "Cannon.h"
#include "TerrainBlock.h"

Stage2::Stage2() : Stage()
{
	mapFilePath = "Resources/Maps/stage2.json";
	TerrainBlock* wallL = new TerrainBlock();
	TerrainBlock* wallR = new TerrainBlock();
	TerrainBlock* wallB = new TerrainBlock();
	wallL->type = TERRAIN_BLOCK_TYPE::WALL;
	wallR->type = TERRAIN_BLOCK_TYPE::WALL;
	wallB->type = TERRAIN_BLOCK_TYPE::WALL;
	wallL->SetW(10.0f);
	wallL->SetH(SCREEN_HEIGHT / SCALING_RATIO_Y);
	wallR->SetW(10.0f);
	wallR->SetH(SCREEN_HEIGHT / SCALING_RATIO_Y);
	wallB->SetW(SCREEN_WIDTH  / SCALING_RATIO_X);
	wallB->SetH(10.0f);
	walls.insert({ "L", wallL });
	walls.insert({ "R", wallR });
	walls.insert({ "B", wallB });
}

Stage2::~Stage2()
{
}
/*
== List nhan vat stage 3 ==
	failingstone 		done
	soldiershotr 		done
	soldiershotl 		done
	soldiershotr 		done
	staticweaponf 		done
	staticweaponl 		done
	staticweapons 		done
	sniper				done
	scubarsolider1 		done
	scubarsolider2 		done
	scubarsolider3 		done
	capsuleweapon3r 	done
	capsuleweapon3b 	done
	gunrotating3 		done
	blazerR 			done
	blazerL 			done
	dynamicbridge1		done
	dynamicbridge2		done
	dynamicbridge3 		done
	cannon3 			done

	boss2finalhead		done
	boss2finalarmleft	done
	boss2finalarmright	done
*/
void Stage2::LoadEntities(void *entitiesLayer)
{
	auto _entitiesLayer = (tson::Layer *)entitiesLayer;
	auto mapH = _entitiesLayer->getMap()->getSize().y * _entitiesLayer->getMap()->getTileSize().y;

	for (auto &object : _entitiesLayer->getObjects())
	{
		auto &position = object.getPosition();
		auto &size = object.getSize();
		Entity *entity = NULL;

		if (object.getName() == "respawnposition")
		{
			bill->SetX(position.x + size.x * 0.5f);
			bill->SetY(mapH - position.y - size.y * 1.0f);
		}
		else if (object.getName() == "cameratranslateposition")
		{
		}
		else if (object.getName() == "failingstone")
		{
			entity = new RockFall();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "soldiershotr")
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
		else if (object.getName() == "staticweaponf")
		{
			entity = new Soldier();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "staticweaponl")
		{
			entity = new Falcon(ITEM_TYPE::L);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "staticweapons")
		{
			entity = new Falcon(ITEM_TYPE::S);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "sniper")
		{
			entity = new RifleManStanding();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "scubarsolider1")
		{
			entity = new ScubaSoldier();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "scubarsolider2")
		{
			entity = new ScubaSoldier();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "scubarsolider3")
		{
			entity = new ScubaSoldier();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "capsuleweapon3b")
		{
			entity = new AirCraft(ITEM_TYPE::B, AIRCRAFT_DIRECTION::VERTICAL);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "capsuleweapon3r")
		{
			entity = new AirCraft(ITEM_TYPE::R, AIRCRAFT_DIRECTION::VERTICAL);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "gunrotating3")
		{
			entity = new WallTurret();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "blazerL")
		{
			entity = new Fire();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "blazerR")
		{
			entity = new Fire();
			entity->SetMovingDirection(DIRECTION::RIGHT);
		}
		else if (object.getName() == "dynamicbridge1")
		{
			entity = new RockFly(80, 176);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "dynamicbridge2")
		{
			entity = new RockFly(64, 144);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "dynamicbridge3")
		{
			entity = new RockFly(80, 224);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "cannon3")
		{
			entity = new Cannon();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "boss2finalhead")
		{
			entity = new BossStage3();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "boss2finalgate")
		{
			entity = new BossStage3Gate();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "boss2finalarmleft")
		{
			entity = new BossStage3Hand();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "boss2finalarmright")
		{
			entity = new BossStage3Hand();
			entity->SetMovingDirection(DIRECTION::RIGHT);
		}

		if (!entity)
			continue;

		auto enemy = dynamic_cast<Enemy<Bill> *>(entity);
		if (enemy)
			enemy->SetTarget(bill);

		entity->SetX(position.x + size.x * 0.5f);
		entity->SetY(mapH - position.y - size.y * 1.0f);
		entity->SetW(FLOAT(size.x));
		entity->SetH(FLOAT(size.y));

		entities->Insert(entity);
	}

	// bill
	auto representativeBill = new Bill();
	representativeBill->LoadTextures();
	representativeBill->LoadSprites();
	representativeBill->LoadAnimations();
	Destroy(representativeBill);

	// fire
	auto representativeFire = new Fire();
	representativeFire->LoadTextures();
	representativeFire->LoadSprites();
	representativeFire->LoadAnimations();
	Destroy(representativeFire);

	// static weapon
	auto representativeFalcon = new Falcon(ITEM_TYPE::I);
	representativeFalcon->LoadTextures();
	representativeFalcon->LoadSprites();
	representativeFalcon->LoadAnimations();
	Destroy(representativeFalcon);

	// aircraft
	auto representativeAirCraft = new AirCraft(ITEM_TYPE::I, AIRCRAFT_DIRECTION::VERTICAL);
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
	auto representativeRockFLy = new RockFly(0, 0);
	representativeRockFLy->LoadTextures();
	representativeRockFLy->LoadSprites();
	representativeRockFLy->LoadAnimations();
	Destroy(representativeRockFLy);

	// bullet
	auto representativeBullet = new Bullet();
	representativeBullet->LoadTextures();
	representativeBullet->LoadSprites();
	representativeBullet->LoadAnimations();
	Destroy(representativeBullet);

	// Soldier
	auto representativeSoldier = new Soldier();
	representativeSoldier->LoadTextures();
	representativeSoldier->LoadSprites();
	representativeSoldier->LoadAnimations();
	Destroy(representativeSoldier);

	// wallTurret
	auto representativeWallTurret = new WallTurret();
	representativeWallTurret->LoadTextures();
	representativeWallTurret->LoadSprites();
	representativeWallTurret->LoadAnimations();
	Destroy(representativeWallTurret);

	// rifeMan
	auto representativeRifleManStanding = new RifleManStanding();
	representativeRifleManStanding->LoadTextures();
	representativeRifleManStanding->LoadSprites();
	representativeRifleManStanding->LoadAnimations();
	Destroy(representativeRifleManStanding);

	// scuba soldier
	auto representativeScubaSoldier = new ScubaSoldier();
	representativeScubaSoldier->LoadTextures();
	representativeScubaSoldier->LoadSprites();
	representativeScubaSoldier->LoadAnimations();
	Destroy(representativeScubaSoldier);

	// boss stage3 head
	auto representativeBossStage3Head = new BossStage3();
	representativeBossStage3Head->LoadTextures();
	representativeBossStage3Head->LoadSprites();
	representativeBossStage3Head->LoadAnimations();
	Destroy(representativeBossStage3Head);

	// boss stage3 gate
	auto representativeBossStage3Gate = new BossStage3Gate();
	representativeBossStage3Gate->LoadTextures();
	representativeBossStage3Gate->LoadSprites();
	representativeBossStage3Gate->LoadAnimations();
	Destroy(representativeBossStage3Gate);

	// boss stage3 hand
	auto representativeBossStage3Hand = new BossStage3Hand();
	representativeBossStage3Hand->LoadTextures();
	representativeBossStage3Hand->LoadSprites();
	representativeBossStage3Hand->LoadAnimations();
	Destroy(representativeBossStage3Hand);

	// canon
	auto representativeCannon = new Cannon();
	representativeCannon->LoadTextures();
	representativeCannon->LoadSprites();
	representativeCannon->LoadAnimations();
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
