#include "Bill.h" 
#include "Item.h"
#include "Stage.h"
#include "Enemy.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Falcon.h"
#include "Camera.h"
#include "Soldier.h"
#include "AirCraft.h"
#include "Explosion.h"
#include "tileson.hpp"
#include "BossStage3.h"
#include "TerrainBlock.h"
#include "TerrainStage1.h"
#include "TerrainStage2.h"
#include "GunBossStage1.h"
#include "BossStage3Hand.h"
#include "FinalBossStage1.h"

Stage:: Stage() : hasDone(0), checkPoint(NULL), mapFilePath(""), translateX(0.0f), translateY(0.0f), bill(NULL), tileW(0.0f), tileH(0.0f), camera(NULL), entities(NULL), backgroundTerrains(NULL), foregroundTerrains(NULL)
{
}


Stage::~Stage()
{
	entities->Clean();
	backgroundTerrains->Clean();
	foregroundTerrains->Clean();
	for (auto& effectEntity : effectEntities) Destroy(effectEntity); effectEntities.clear();
	Destroy(camera); Destroy(entities); Destroy(backgroundTerrains), Destroy(foregroundTerrains);
}


void Stage::Update()
{
	if (hasDone)
		return;


	//if (bill->GetY() == +std::numeric_limits<FLOAT>::infinity())
	//{
	//	if (dynamic_cast<Stage1*>(this))
	//	{
	//		bill->SetX(camera->GetL() + bill->GetW() * 2.0f);
	//		bill->SetY(camera->GetT() - bill->GetH() * 1.0f);
	//	}
	//	else
	//	if (dynamic_cast<Stage2*>(this))
	//	{
	//		bill->SetX(camera->GetL() + bill->GetW() * 2.0f);
	//		bill->SetY(camera->GetY() - bill->GetH() * 1.0f);
	//	}
	//}


	SetRevivalPoint();


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


	std::list<Entity*> deadEntities;
	std::list<Entity*> outOfBoundBullets;
	for (auto& [entity, node] : entitiesResult)
	{
		if (entity && entity == bill)
		{
			continue;
		}
		if (ProcessSpecialEntity(entity))
		{
			continue;
		}
		if (entity && entity->isDead)
		{
			node->entities.remove (entity);
			deadEntities.push_back(entity);
			continue;
		}
		if (dynamic_cast<Bullet*>(entity))
		{
			if (!camera->CouldSee(entity))
			{
				node->entities.remove      (entity);
				outOfBoundBullets.push_back(entity);
			}
		}
	}
	for (auto& deadEntity : deadEntities)
	{
		if (auto enemy = dynamic_cast<Enemy<Bill>*>(deadEntity))
		{
			Explosion* explosion = NULL;
			switch (enemy->enemyType)
			{
			
			case ENEMY_TYPE::NONE:
			break;

			case ENEMY_TYPE::BOSS:
				 explosion = new Explosion(new ExplosionType3State());
			break;

			case ENEMY_TYPE::HUMAN:
				 Sound::getInstance()->play("qexplode", false, 1);
				 if (deadEntity->isDrown)
				 {
					 explosion = new Explosion(new ExplosionDrownState());
				 }
				 else 
				 {
					 explosion = new Explosion(new ExplosionType1State());
				 }
			break;

			case ENEMY_TYPE::MACHINE:
				 Sound::getInstance()->play("exbullet", false, 1);
				 explosion = new Explosion(new ExplosionType2State());
			break;

			}
			if (explosion)
			{
				explosion->SetX(deadEntity->GetX());
				explosion->SetY(deadEntity->GetY());
				if (deadEntity->isDrown) explosion->SetY(explosion->GetY() - 20.0f);
				effectEntities.push_back(explosion);
				ProcessSpecialExplosion(deadEntity);
			}
		}
		else
		if (auto bullet = dynamic_cast<Bullet*>(deadEntity))
		{
			if (ProcessSpecialBullet(bullet))
			{

			}
			else
			{
				Bullet* explosion = new Bullet();
				explosion->SetState(new BulletExplodeState());
				if (bullet->GetVX() < 0.0f)
					explosion->SetX(bullet->GetL() - 3.0f);
				else
				if (bullet->GetVX() > 0.0f)
					explosion->SetX(bullet->GetR() + 3.0f);
				else 
					explosion->SetX(bullet->GetX() + 0.0f);
				if (bullet->GetVY() < 0.0f)
					explosion->SetY(bullet->GetB() - 3.0f);
				else
				if (bullet->GetVY() > 0.0f)
					explosion->SetY(bullet->GetT() + 3.0f);
				else
					explosion->SetY(bullet->GetY() + 0.0f);
				effectEntities.push_back(explosion);
			}
		}
		if (auto falcon = dynamic_cast<Falcon*>(deadEntity))
		{
			Item* item = new Item(falcon->getAmmoType());
			item->SetX(falcon->GetX());
			item->SetY(falcon->GetY());
			entities->Insert(item);
		}
		else 
		if (auto aircraft = dynamic_cast<AirCraft*>(deadEntity))
		{
			Item* item = new Item(aircraft->getAmmoType());
			item->SetX(aircraft->GetX());
			item->SetY(aircraft->GetY());
			entities->Insert(item);
		}
		entitiesResult.erase(deadEntity);
		Destroy             (deadEntity);
	}
	for (auto& outOfBoundBullet : outOfBoundBullets)
	{
		entitiesResult.erase(outOfBoundBullet);
		Destroy             (outOfBoundBullet);
	}
	for (auto& effectEntity : effectEntities)
	{
		if (effectEntity->isDead)
		{
			Destroy(effectEntity);
		}
		else
		{
			effectEntity->Update();
		}
	}
	effectEntities.remove_if([](Entity* effectEntity) { return effectEntity == NULL; });


	if (QuadTreeNode::Update(entities, entitiesResult))
	{
		entitiesResult.clear();
		entities->Retrieve(camera, entitiesResult);
	}


	TranslateCamera();
	TranslateWalls ();
	CheckIfHasDone ();
}


void Stage::Render()
{
	backgroundTerrainsResult.clear();
	backgroundTerrains->Retrieve(camera,   backgroundTerrainsResult);
	for (auto& [backgroundTerrain, node] : backgroundTerrainsResult) backgroundTerrain->Render();


	if (hasDone)
	{
		if (auto stage1 = dynamic_cast<Stage1*>(this))
			     stage1-> finalBossStage1->Render();
		return;
	}


	for (auto& [entity           , node] : entitiesResult)                      entity->Render();
	for (auto&  effectEntity             : effectEntities)                effectEntity->Render();
	bill->Render();
}


void Stage::HandleInput(Input& input)
{
	bill->HandleInput(input);
}


void Stage::CheckResolveClearCollision()
{
	if (hasDone)
		return;


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


	//for (auto& [entity, node] : entitiesResult)
	//{
	//	if (bill != entity)
	//	    bill->CollideWith(entity);
	//}


	for (auto& [entity1, node1] : entitiesResult)
	{
		auto collidableEntity = dynamic_cast<CollidableEntity*>(entity1);
		if  (collidableEntity)
		{
			for (auto& [entity2, node2] : entitiesResult)
			{
				if (entity1 != entity2)
				{
					if (auto bossStage3Hand = dynamic_cast<BossStage3Hand*>(entity2))
					{
						collidableEntity->CollideWith(bossStage3Hand);
						for (auto& bossStage3Joint :  bossStage3Hand->joints)
							if    (bossStage3Joint)
								   collidableEntity->CollideWith(bossStage3Joint);
					}
					else
					{
						collidableEntity->CollideWith(entity2);
					}
				}
			}
			if (dynamic_cast<Soldier*>(collidableEntity))
			{
				for (auto& [name, wall] : walls)
				{
					collidableEntity->CollideWith(wall);
				}
			}
		}
	}


	for (auto& [name, wall] : walls)
	{
		bill->CollideWith(wall);
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
		if (object.getClassType() == "check_point")
		foregroundTerrain->type = TERRAIN_BLOCK_TYPE::CHECK_POINT;
		if (object.getClassType() == "water")
		foregroundTerrain->type = TERRAIN_BLOCK_TYPE::WATER;

		foregroundTerrains->Insert(foregroundTerrain);

		if (foregroundTerrain 
		&&  foregroundTerrain->type == TERRAIN_BLOCK_TYPE::CHECK_POINT)
			checkPoint = foregroundTerrain;
	}
}


