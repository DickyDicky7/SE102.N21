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
#include "TerrainBlock.h"
#include "Bridge.h"
#include "GunBossStage1.h"
#include "FinalBossStage1.h"

Stage1::Stage1() : Stage()
{
	mapFilePath = "Resources/Maps/stage1.json";
	TerrainBlock* wallL = new TerrainBlock();
	TerrainBlock* wallR = new TerrainBlock();
	TerrainBlock* wallB = new TerrainBlock();
	wallL->type = TERRAIN_BLOCK_TYPE::WALL;
	wallR->type = TERRAIN_BLOCK_TYPE::WALL;
	wallB->type = TERRAIN_BLOCK_TYPE::WALL;
	wallL->SetW(20.0f);
	wallL->SetH(SCREEN_HEIGHT / SCALING_RATIO_Y);
	wallR->SetW(20.0f);
	wallR->SetH(SCREEN_HEIGHT / SCALING_RATIO_Y);
	wallB->SetW(SCREEN_WIDTH  / SCALING_RATIO_X);
	wallB->SetH(20.0f);
	wallL->name  = "L";
	wallR->name  = "R";
	wallB->name  = "B";
	walls.insert({ "L", wallL });
	walls.insert({ "R", wallR });
	walls.insert({ "B", wallB });

	finalBossStage1 = NULL;
}

Stage1::~Stage1()
{
}

void Stage1::CheckIfHasDone()
{
	if (finalBossStage1->isDead && checkPoint && bill->AABBCheck(checkPoint))
	{
		Sound::getInstance()->play("passboss", false, 1);
		hasDone = 1;
	}
}

void Stage1::TranslateWalls()
{
	for (auto& [name, wall] : walls)
	{
		if (name == "L")
		{
			wall->SetX(camera->GetL());
			wall->SetY(camera->GetB());
		}
		else
		if (name == "R")
		{
			wall->SetX(camera->GetR());
			wall->SetY(camera->GetB());
		}
		else
		if (name == "B")
		{
			wall->SetX(camera->GetX());
			wall->SetY(camera->GetB() - wall->GetH() * 0.5f);
		}
		else
		if (name == "T")
		{
			wall->SetX(camera->GetX());
			wall->SetY(camera->GetT() - wall->GetH() * 0.5f);
		}
	}
}

void Stage1::TranslateCamera()
{
	if (bill && camera && bill->GetX() >= translateX && camera->GetL() <  translateX)
	{
		camera->SetX(camera->GetX() + 1.0f);
		Sound::getInstance()->play("warning", false, 1);
	}
	else
	if (bill && camera && bill->GetX() >= translateX && camera->GetL() >= translateX)
	{
		if (!camera->isStatic)
			 camera->ToStatic();
	}
}

void Stage1::SetRevivalPoint()
{
	if (bill->GetY() == +std::numeric_limits<FLOAT>::infinity())
	{
		std::vector<std::pair<Entity*, QuadTreeNode*>> 
			 sortedForegroundTerrainsResult(foregroundTerrainsResult.begin(), foregroundTerrainsResult.end());
		std::sort
		(
			sortedForegroundTerrainsResult.begin(), sortedForegroundTerrainsResult.end(),
			[](std::pair<Entity*, QuadTreeNode*> pair1, std::pair<Entity*, QuadTreeNode*> pair2) -> BOOL
			{
				return pair1.first->GetL() < pair2.first->GetL();
			}
		);

		BOOL hasForegroundTerrain = 0;
		FLOAT W = bill->GetW() * 2.0f;
		FLOAT H = bill->GetH() * 1.0f;
		for (auto& [foregroundTerrain, node] : sortedForegroundTerrainsResult)
		{
			if (foregroundTerrain->GetR() - camera->GetL() > W)
			{
				hasForegroundTerrain = 1;
				if (camera->GetL() >= foregroundTerrain->GetL()) bill->SetX(           camera->GetL() + W);
				                                            else bill->SetX(foregroundTerrain->GetL() + W);
				break;
			}
		}
		if (!hasForegroundTerrain)
			 bill->SetX(camera->GetL() + W);
		     bill->SetY(camera->GetT() - H);
	}
}

BOOL Stage1::ProcessSpecialEntity(Entity* entity)
{
	if (auto    gunBossStage1 = dynamic_cast<  GunBossStage1*>(entity))
	{
		return 1;
	}

	if (auto  finalBossStage1 = dynamic_cast<FinalBossStage1*>(entity))
	{
		if (! finalBossStage1->isDead)
			  return 1;
		
		if (++finalBossStage1->deadTurns != 5)
			  return 1;

		Sound::getInstance()->stop();
		Sound::getInstance()->play("boss1dead", false, 1);

		FLOAT X = finalBossStage1->GetL() + finalBossStage1->GetW() * 0.25f;
		FLOAT Y = finalBossStage1->GetT() - finalBossStage1->GetH() * 0.25f;
		for (int i = 0; i <= 10; i++)
		{
			Explosion* subExplosion1 = new Explosion(new ExplosionType3State());
			Explosion* subExplosion2 = new Explosion(new ExplosionType3State());
			subExplosion1->SetX(X + i * finalBossStage1->GetW() * 0.75f);
			subExplosion1->SetY(Y                                      );
			subExplosion2->SetX(X + i * finalBossStage1->GetW() * 0.75f);
			subExplosion2->SetY(Y -     finalBossStage1->GetH() * 0.75f);
			effectEntities.push_back(subExplosion1);
			effectEntities.push_back(subExplosion2);
		}
		return 1;
	}

	return 0;
}

BOOL Stage1::ProcessSpecialBullet(Bullet* bullet)
{
	if (dynamic_cast<BulletBossStage1State*>(bullet->GetState()))
	{
		Explosion* explosion = new Explosion(new ExplosionType2State());
		explosion->SetX(bullet->GetX()       );
		explosion->SetY(bullet->GetY() - 5.0f);
		effectEntities.push_back(explosion);
		return 1;
	}

	return 0;
}

BOOL Stage1::ProcessSpecialExplosion(Entity* deadEntity)
{
	return 0;
}

void Stage1::LoadEntities(void *entitiesLayer)
{
	auto _entitiesLayer = (tson::Layer*)entitiesLayer;
	auto mapH = _entitiesLayer->getMap()->getSize().y * _entitiesLayer->getMap()->getTileSize().y;

	GunBossStage1* bossGun1 = new GunBossStage1(1);
	GunBossStage1* bossGun2 = new GunBossStage1(2);

	FinalBossStage1* finalBoss = new FinalBossStage1();

	finalBoss->SetGun1(bossGun1);
	finalBoss->SetGun2(bossGun2);

	finalBossStage1 = finalBoss;

	for (auto& object : _entitiesLayer->getObjects())
	{
		auto& position = object.getPosition();
		auto& size = object.getSize();
		Entity* entity = NULL;

		if (object.getName() == "bridge")
		{
			// this special cause i dont want to init bridge part when update bridge
			D3DXVECTOR3 bridgePos;
			bridgePos.x = position.x + size.x * 0.5f;
			bridgePos.y = mapH - position.y - size.y * 1.0f;

			entity = new Bridge(bridgePos, FLOAT(size.x));
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "sniper")
		{
			entity = new RifleManStanding();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "sniperh")
		{
			entity = new RifleManHideOnBush();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "snipere")
		{
			entity = new RifleManStanding();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "cannon1")
		{
			entity = new Cannon();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "soldierl")
		{
			entity = new Soldier();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "gunboss1")
		{
			entity = bossGun1;
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "gunboss2")
		{
			entity = bossGun2;
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "finalboss1")
		{
			entity = finalBoss;
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "gunrotating1")
		{
			entity = new WallTurret();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "staticweaponm")
		{
			entity = new Falcon(ITEM_TYPE::M);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "staticweaponf")
		{
			entity = new Falcon(ITEM_TYPE::F);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "staticweapons")
		{
			entity = new Falcon(ITEM_TYPE::S);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "capsuleweaponr")
		{
			entity = new AirCraft(ITEM_TYPE::R, AIRCRAFT_DIRECTION::HORIZONTAL);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "capsuleweaponl")
		{
			entity = new AirCraft(ITEM_TYPE::L, AIRCRAFT_DIRECTION::HORIZONTAL);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "respawnposition")
		{
			bill->SetX(position.x + size.x * 0.5f);
			bill->SetY(mapH - position.y - size.y * 1.0f);
		}
		else if (object.getName() == "cameratranslateposition")
		{
			translateX = position.x + size.x * 0.5f;
			translateY = mapH - position.y - size.y * 1.0f;
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

	auto representativeBill = new Bill();
	auto representativeBullet = new Bullet();
	auto representativeFalcon = new Falcon(ITEM_TYPE::I);
	auto representativeSoldier = new Soldier();
	auto representativeAirCraft = new AirCraft(ITEM_TYPE::I, AIRCRAFT_DIRECTION::HORIZONTAL);
	auto representativeWallTurret = new WallTurret();
	auto representativeRifleManStanding = new RifleManStanding();
	auto representativeRifleManHideOnBush = new RifleManHideOnBush();
	auto representativeCannon = new Cannon();
	auto representativeBridge = new Bridge();

	representativeBill->LoadTextures();
	representativeFalcon->LoadTextures();
	representativeBullet->LoadTextures();
	representativeSoldier->LoadTextures();
	representativeAirCraft->LoadTextures();
	representativeWallTurret->LoadTextures();
	representativeRifleManStanding->LoadTextures();
	representativeRifleManHideOnBush->LoadTextures();
	representativeCannon->LoadTextures();
	representativeBridge->LoadTextures();
	bossGun1->LoadTextures();
	finalBoss->LoadTextures();

	representativeBill->LoadSprites();
	representativeFalcon->LoadSprites();
	representativeBullet->LoadSprites();
	representativeSoldier->LoadSprites();
	representativeAirCraft->LoadSprites();
	representativeWallTurret->LoadSprites();
	representativeRifleManStanding->LoadSprites();
	representativeRifleManHideOnBush->LoadSprites();
	representativeCannon->LoadSprites();
	representativeBridge->LoadSprites();
	bossGun1->LoadSprites();
	finalBoss->LoadSprites();

	representativeBill->LoadAnimations();
	representativeFalcon->LoadAnimations();
	representativeBullet->LoadAnimations();
	representativeSoldier->LoadAnimations();
	representativeAirCraft->LoadAnimations();
	representativeWallTurret->LoadAnimations();
	representativeRifleManStanding->LoadAnimations();
	representativeRifleManHideOnBush->LoadAnimations();
	representativeCannon->LoadAnimations();
	representativeBridge->LoadAnimations();
	bossGun1->LoadAnimations();
	finalBoss->LoadAnimations();

	Destroy(representativeBill);
	Destroy(representativeFalcon);
	Destroy(representativeBullet);
	Destroy(representativeSoldier);
	Destroy(representativeAirCraft);
	Destroy(representativeWallTurret);
	Destroy(representativeRifleManStanding);
	Destroy(representativeRifleManHideOnBush);
	Destroy(representativeCannon);
	Destroy(representativeBridge);

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
