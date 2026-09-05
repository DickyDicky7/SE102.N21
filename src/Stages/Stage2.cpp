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
	this->_mapFilePath = "Resources/Maps/stage2.json";
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
}

Stage2::~Stage2()
{
}

void Stage2::CheckIfHasDone()
{
	if (this->_bill && this->_checkPoint && !this->_hasDone)
	{
		if (this->_entitiesResult.size() <= 1 && this->_bill->AABBCheck(this->_checkPoint) && this->_bill->GetVY() <= Constants::Stages::CHECKPOINT_FALL_THRESHOLD)
		{
			Sound::GetInstance()->Play("passboss", false, 1);
			this->_hasDone = true;
		}
	}
}

void Stage2::TranslateWalls()
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
			//if (_camera->GetY() < SCREEN_HEIGHT / SCALING_RATIO_Y) wall->SetY(_camera->GetB() - wall->GetH() * 0.5f);
			//                                                 else wall->SetY(_camera->GetB() - wall->GetH()       );
			wall->SetY(this->_camera->GetB() - wall->GetH() * Constants::Stages::Stage2::WALL_CAMERA_BOTTOM_OFFSET_RATIO);
		}
		else
		if (name == "T")
		{
			wall->SetX(this->_camera->GetX());
			wall->SetY(this->_camera->GetT() - wall->GetH()       );
		}
	}
}

void Stage2::TranslateCamera()
{
	if (this->_bill && this->_camera && this->_bill->GetY() >= this->_translateY && this->_camera->GetB() <  this->_translateY)
	{
		this->_camera->SetY(this->_camera->GetY() + Constants::Stages::CAMERA_TRANSLATE_STEP);
	}
	else
	if (this->_bill && this->_camera && this->_bill->GetY() >= this->_translateY && this->_camera->GetB() >= this->_translateY)
	{
		if (!this->_camera->IsStatic())
			 this->_camera->ToStatic();
	}
}

void Stage2::SetRevivalPoint()
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
		float W = this->_bill->GetW() * Constants::Stages::Stage2::BILL_SPAWN_OFFSET_RATIO_X;
		float H = this->_bill->GetH() * Constants::Stages::Stage2::BILL_SPAWN_OFFSET_RATIO_Y;
		for (auto& [foregroundTerrain, node] : sortedForegroundTerrainsResult)
		{
			if (this->_camera->GetY() - foregroundTerrain->GetT() > H)
			{
				this->_bill->SetX(foregroundTerrain->GetL() + W);
				hasForegroundTerrain = true;
				break;
			}
		}
		if (!hasForegroundTerrain)
		{
			this->_bill->SetX(this->_camera->GetL() + W);
		}
		this->_bill->SetY(this->_camera->GetY() - H);
	}
}

bool Stage2::ProcessSpecialEntity(Entity* entity)
{
	return false;
}

bool Stage2::ProcessSpecialBullet(Bullet* bullet)
{
	return false;
}

bool Stage2::ProcessSpecialExplosion(Entity* deadEntity)
{
	if (deadEntity)
	{
		deadEntity->ProcessSpecialDeathEffects(this->_effectEntities);
	}
	return false;
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
void Stage2::LoadEntities(void* entitiesLayer)
{
	auto entitiesLyr = static_cast<tson::Layer*>(entitiesLayer);
	auto mapH = entitiesLyr->getMap()->getSize().y * entitiesLyr->getMap()->getTileSize().y;

	BossStage3Hand* boss3Stage3HandLeft = new BossStage3Hand();
	BossStage3Hand* boss3Stage3HandRight = new BossStage3Hand();
	BossStage3Gate* boss3StageGate = new BossStage3Gate();
	BossStage3* boss3StageHead = new BossStage3();

	boss3StageHead->SetHandRight(boss3Stage3HandRight);
	boss3StageHead->SetHandLeft(boss3Stage3HandLeft);
	boss3StageGate->SetHead(boss3StageHead);

	for (auto& object : entitiesLyr->getObjects())
	{
		auto &position = object.getPosition();
		auto &size = object.getSize();
		Entity *entity = nullptr;

		if (object.getName() == "respawnposition")
		{
			this->_bill->SetX(position.x + size.x * 0.5f);
			this->_bill->SetY(mapH - position.y - size.y * 1.0f);
		}
		else if (object.getName() == "cameratranslateposition")
		{
			this->_translateX = position.x + size.x * 0.5f;
			this->_translateY = mapH - position.y - size.y * 1.0f;
		}
		else if (object.getName() == "failingstone")
		{
			entity = new RockFall();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "soldiershotr")
		{
			auto soldier = new Soldier();
			soldier->SetShootable(true);
			entity = soldier;
			entity->SetMovingDirection(DIRECTION::RIGHT);
		}
		else if (object.getName() == "soldiershotl")
		{
			auto soldier = new Soldier();
			soldier->SetShootable(true);
			entity = soldier;
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "staticweaponf")
		{
			entity = new Falcon(ITEM_TYPE::F);
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
			entity = new RockFly(Constants::Stages::Stage2::ROCKFLY_SPAWN_1_BEGIN, Constants::Stages::Stage2::ROCKFLY_SPAWN_1_END);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "dynamicbridge2")
		{
			entity = new RockFly(Constants::Stages::Stage2::ROCKFLY_SPAWN_2_BEGIN, Constants::Stages::Stage2::ROCKFLY_SPAWN_2_END);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "dynamicbridge3")
		{
			entity = new RockFly(Constants::Stages::Stage2::ROCKFLY_SPAWN_3_BEGIN, Constants::Stages::Stage2::ROCKFLY_SPAWN_3_END);
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "cannon3")
		{
			entity = new Cannon();
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "boss2finalhead")
		{
			entity = boss3StageHead;
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "boss2finalgate")
		{
			entity = boss3StageGate;
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "boss2finalarmleft")
		{
			entity = boss3Stage3HandLeft;
			entity->SetMovingDirection(DIRECTION::LEFT);
		}
		else if (object.getName() == "boss2finalarmright")
		{
			entity = boss3Stage3HandRight;
			entity->SetMovingDirection(DIRECTION::RIGHT);
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
	Stage::PreloadResources<Fire>();
	Stage::PreloadResources<Falcon>(ITEM_TYPE::I);
	Stage::PreloadResources<AirCraft>(ITEM_TYPE::I, AIRCRAFT_DIRECTION::VERTICAL);
	Stage::PreloadResources<RockFall>();
	Stage::PreloadResources<RockFly>(0.0f, 0.0f);
	Stage::PreloadResources<Bullet>();
	Stage::PreloadResources<Soldier>();
	Stage::PreloadResources<WallTurret>();
	Stage::PreloadResources<RifleManStanding>();
	Stage::PreloadResources<ScubaSoldier>();
	Stage::PreloadResources<BossStage3>();
	Stage::PreloadResources<BossStage3Gate>();
	Stage::PreloadResources<BossStage3Hand>();
	Stage::PreloadResources<Cannon>();
	Stage::PreloadResources<Explosion>();
	Stage::PreloadResources<Item>(ITEM_TYPE::I);
}
