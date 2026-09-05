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
	this->_mapFilePath = "Resources/Maps/stage1.json";
	TerrainBlock* wallL = new TerrainBlock();
	TerrainBlock* wallR = new TerrainBlock();
	TerrainBlock* wallB = new TerrainBlock();
	wallL->SetTerrainType(TERRAIN_BLOCK_TYPE::WALL);
	wallR->SetTerrainType(TERRAIN_BLOCK_TYPE::WALL);
	wallB->SetTerrainType(TERRAIN_BLOCK_TYPE::WALL);
	wallL->SetW(Constants::Stages::BOUNDARY_WALL_THICKNESS);
	wallL->SetH(Constants::Screen::IN_GAME_HEIGHT);
	wallR->SetW(Constants::Stages::BOUNDARY_WALL_THICKNESS);
	wallR->SetH(Constants::Screen::IN_GAME_HEIGHT);
	wallB->SetW(Constants::Screen::IN_GAME_WIDTH);
	wallB->SetH(Constants::Stages::BOUNDARY_WALL_THICKNESS);
	wallL->SetEntityName("L");
	wallR->SetEntityName("R");
	wallB->SetEntityName("B");
	this->_walls.insert({ "L", wallL });
	this->_walls.insert({ "R", wallR });
	this->_walls.insert({ "B", wallB });

	this->_finalBossStage1 = nullptr;
}

Stage1::~Stage1()
{
}

void Stage1::CheckIfHasDone()
{
	// _finalBossStage1 is only assigned by LoadEntities, and CheckIfHasDone runs
	// from Stage::Update; RenderBossCompletion already guards the same pointer.
	if (this->_finalBossStage1 && this->_finalBossStage1->IsDead() && this->_checkPoint && this->_bill && this->_bill->AABBCheck(this->_checkPoint))
	{
		Sound::GetInstance()->Play("passboss", false, 1);
		this->_hasDone = true;
	}
}

void Stage1::RenderBossCompletion()
{
	if (this->_finalBossStage1)
		this->_finalBossStage1->Render();
}

void Stage1::TranslateWalls()
{
	for (auto& [name, wall] : this->_walls)
	{
		if (name == "L")
		{
			wall->SetX(this->_camera->GetL());
			wall->SetY(this->_camera->GetB());
		}
		else
		if (name == "R")
		{
			wall->SetX(this->_camera->GetR());
			wall->SetY(this->_camera->GetB());
		}
		else
		if (name == "B")
		{
			wall->SetX(this->_camera->GetX());
			wall->SetY(this->_camera->GetB() - wall->GetH() * 0.5f);
		}
		else
		if (name == "T")
		{
			wall->SetX(this->_camera->GetX());
			wall->SetY(this->_camera->GetT() - wall->GetH() * 0.5f);
		}
	}
}

void Stage1::TranslateCamera()
{
	if (this->_bill && this->_camera && this->_bill->GetX() >= this->_translateX && this->_camera->GetL() <  this->_translateX)
	{
		this->_camera->SetX(this->_camera->GetX() + Constants::Stages::CAMERA_TRANSLATE_STEP);
		Sound::GetInstance()->Play("warning", false, 1);
	}
	else
	if (this->_bill && this->_camera && this->_bill->GetX() >= this->_translateX && this->_camera->GetL() >= this->_translateX)
	{
		if (!this->_camera->IsStatic())
			 this->_camera->ToStatic();
	}
}

void Stage1::SetRevivalPoint()
{
	if (this->_bill->GetY() == +std::numeric_limits<float>::infinity())
	{
		std::vector<std::pair<Entity*, QuadTreeNode*>>
			 sortedForegroundTerrainsResult(this->_foregroundTerrainsResult.begin(), this->_foregroundTerrainsResult.end());
		std::sort
		(
			sortedForegroundTerrainsResult.begin(), sortedForegroundTerrainsResult.end(),
			[](std::pair<Entity*, QuadTreeNode*> pair1, std::pair<Entity*, QuadTreeNode*> pair2) -> bool
			{
				return pair1.first->GetL() < pair2.first->GetL();
			}
		);

		bool hasForegroundTerrain = false;
		float W = this->_bill->GetW() * Constants::Stages::Stage1::BILL_SPAWN_OFFSET_FACTOR_W;
		float H = this->_bill->GetH() * Constants::Stages::REVIVAL_DEFAULT_HEIGHT_FACTOR;
		for (auto& [foregroundTerrain, node] : sortedForegroundTerrainsResult)
		{
			if (foregroundTerrain->GetR() - this->_camera->GetL() > W)
			{
				hasForegroundTerrain = true;
				if (this->_camera->GetL() >= foregroundTerrain->GetL()) this->_bill->SetX(           this->_camera->GetL() + W);
				                                                  else this->_bill->SetX(foregroundTerrain->GetL() + W);
				break;
			}
		}
		if (!hasForegroundTerrain)
		{
			this->_bill->SetX(this->_camera->GetL() + W);
		}
		this->_bill->SetY(this->_camera->GetT() - H);
	}
}

bool Stage1::ProcessSpecialEntity(Entity* entity)
{
	if (!entity)
		return false;

	if (entity->ShouldRetainWhenDead())
		return true;

	if (entity == this->_finalBossStage1)
	{
		if (!this->_finalBossStage1->IsDead())
			return true;

		this->_finalBossStage1->IncrementDeadTurns();
		if (this->_finalBossStage1->GetDeadTurns() != Constants::Enemies::BossStage1::FinalBoss::DEAD_TURN_COUNT)
			return true;

		Sound::GetInstance()->Stop();
		Sound::GetInstance()->Play("boss1dead", false, 1);

		float X = this->_finalBossStage1->GetL() + this->_finalBossStage1->GetW() * Constants::Enemies::BossStage1::FINAL_BOSS_EXPLOSION_OFFSET_RATIO_X;
		float Y = this->_finalBossStage1->GetT() - this->_finalBossStage1->GetH() * Constants::Enemies::BossStage1::FINAL_BOSS_EXPLOSION_OFFSET_RATIO_Y;
		for (int i = 0; i < Constants::Enemies::BossStage1::FinalBoss::SUB_EXPLOSIONS_TOTAL_COUNT; i++)
		{
			Explosion* subExplosion1 = new Explosion(new ExplosionType3State());
			Explosion* subExplosion2 = new Explosion(new ExplosionType3State());
			subExplosion1->SetX(X + i * this->_finalBossStage1->GetW() * Constants::Enemies::BossStage1::FINAL_BOSS_EXPLOSION_SPACING_RATIO_X);
			subExplosion1->SetY(Y                                      );
			subExplosion2->SetX(X + i * this->_finalBossStage1->GetW() * Constants::Enemies::BossStage1::FINAL_BOSS_EXPLOSION_SPACING_RATIO_X);
			subExplosion2->SetY(Y -     this->_finalBossStage1->GetH() * Constants::Enemies::BossStage1::FINAL_BOSS_EXPLOSION_SPACING_RATIO_Y);
			this->_effectEntities.push_back(subExplosion1);
			this->_effectEntities.push_back(subExplosion2);
		}
		return true;
	}

	return false;
}

bool Stage1::ProcessSpecialBullet(Bullet* bullet)
{
	return false;
}

bool Stage1::ProcessSpecialExplosion(Entity* deadEntity)
{
	return false;
}

void Stage1::LoadEntities(void* entitiesLayer)
{
	auto entitiesLyr = static_cast<tson::Layer*>(entitiesLayer);
	auto mapH = entitiesLyr->getMap()->getSize().y * entitiesLyr->getMap()->getTileSize().y;

	GunBossStage1* bossGun1 = new GunBossStage1(Constants::Enemies::BossStage1::Gun::TYPE_UPPER);
	GunBossStage1* bossGun2 = new GunBossStage1(Constants::Enemies::BossStage1::Gun::TYPE_LOWER);

	FinalBossStage1* finalBoss = new FinalBossStage1();

	finalBoss->SetGun1(bossGun1);
	finalBoss->SetGun2(bossGun2);

	this->_finalBossStage1 = finalBoss;

	for (auto& object : entitiesLyr->getObjects())
	{
		auto& position = object.getPosition();
		auto& size = object.getSize();
		Entity* entity = nullptr;

		if (object.getName() == "bridge")
		{
			// this special cause i dont want to init bridge part when update bridge
			D3DXVECTOR3 bridgePos;
			bridgePos.x = position.x + size.x * 0.5f;
			bridgePos.y = mapH - position.y - size.y * 1.0f;

			entity = new Bridge(bridgePos, static_cast<float>(size.x));
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
			this->_bill->SetX(position.x + size.x * 0.5f);
			this->_bill->SetY(mapH - position.y - size.y * 1.0f);
		}
		else if (object.getName() == "cameratranslateposition")
		{
			this->_translateX = position.x + size.x * 0.5f;
			this->_translateY = mapH - position.y - size.y * 1.0f;
		}

		if (!entity)
			continue;

		entity->SetTarget(this->_bill);

		entity->SetX(position.x + size.x * 0.5f);
		entity->SetY(mapH - position.y - size.y * 1.0f);
		entity->SetW(static_cast<float>(size.x));
		entity->SetH(static_cast<float>(size.y));

		this->_entities->Insert(entity);
	}

	Stage::PreloadResources<Bill>();
	Stage::PreloadResources<Bullet>();
	Stage::PreloadResources<Falcon>(ITEM_TYPE::I);
	Stage::PreloadResources<Soldier>();
	Stage::PreloadResources<AirCraft>(ITEM_TYPE::I, AIRCRAFT_DIRECTION::HORIZONTAL);
	Stage::PreloadResources<WallTurret>();
	Stage::PreloadResources<RifleManStanding>();
	Stage::PreloadResources<RifleManHideOnBush>();
	Stage::PreloadResources<Cannon>();
	Stage::PreloadResources<Bridge>();
	Stage::PreloadResources<Explosion>();
	Stage::PreloadResources<Item>(ITEM_TYPE::I);

	bossGun1->LoadTextures();
	bossGun1->LoadSprites();
	bossGun1->LoadAnimations();

	finalBoss->LoadTextures();
	finalBoss->LoadSprites();
	finalBoss->LoadAnimations();
}
