#include "Bill.h"
#include "Item.h"
#include "Stage.h"
#include "Enemy.h"
#include "Bullet.h"
#include "ParticleSystem.h"
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

Stage:: Stage() : _hasDone(false), _hasUpdated(false), _checkPoint(nullptr), _mapFilePath(""), _translateX(0.0f), _translateY(0.0f), _bill(nullptr), _tileW(0.0f), _tileH(0.0f), _camera(nullptr), _entities(nullptr), _backgroundTerrains(nullptr), _foregroundTerrains(nullptr)
{
}


Stage::~Stage()
{
	// The three trees are only built by Load(); a stage destroyed before that
	// runs - a map that fails to parse, a construction that is abandoned - still
	// gets here, and these were unconditional dereferences of the NULLs the
	// constructor left.
	if (this->_entities)           this->_entities->Clean();
	if (this->_backgroundTerrains) this->_backgroundTerrains->Clean();
	if (this->_foregroundTerrains) this->_foregroundTerrains->Clean();
	for (auto& effectEntity : this->_effectEntities) Destroy(effectEntity); this->_effectEntities.clear();
	for (auto& [name, wall] : this->_walls) Destroy(wall); this->_walls.clear();
	Destroy(this->_camera); Destroy(this->_entities); Destroy(this->_backgroundTerrains); Destroy(this->_foregroundTerrains);
	BulletParticleSystem::Clear();
	auto& bullets = HasWeapons::GetBullets();
	for (auto& bullet : bullets) Destroy(bullet);
	bullets.clear();
}


void Stage::Update()
{
	if (this->_hasDone)
		return;

	this->SetRevivalPoint();


	auto& bullets = HasWeapons::GetBullets();
	for (auto& bullet : bullets)
	{
		if (this->_camera->CouldSee(bullet))
			this->_entities->Insert(bullet); else Destroy(bullet);
	}
	bullets.clear();


	this->_entitiesResult.clear();
	this->_entities->Retrieve(this->_camera, this->_entitiesResult);
	for (auto& [entity, node] : this->_entitiesResult) entity->Update();


	std::vector<Entity*> deadEntities;
	deadEntities.reserve(Constants::Stages::DEFAULT_CANDIDATE_RESERVE_CAPACITY);
	std::vector<Entity*> outOfBoundBullets;
	outOfBoundBullets.reserve(Constants::Stages::DEFAULT_CANDIDATE_RESERVE_CAPACITY);
	for (auto& [entity, node] : this->_entitiesResult)
	{
		if (entity && entity == this->_bill)
		{
			continue;
		}
		if (this->ProcessSpecialEntity(entity))
		{
			continue;
		}
		if (entity && entity->IsDead())
		{
			this->_entities->Remove(entity);
			deadEntities.push_back(entity);
			continue;
		}
		if (entity && entity->IsBullet())
		{
			if (!this->_camera->CouldSee(entity))
			{
				this->_entities->Remove(entity);
				outOfBoundBullets.push_back(entity);
			}
		}
	}
	for (auto& deadEntity : deadEntities)
	{
		if (deadEntity->IsEnemy())
		{
			Explosion* explosion = nullptr;
			switch (deadEntity->GetEnemyType())
			{

			case ENEMY_TYPE::NONE:
			break;

			case ENEMY_TYPE::BOSS:
				 explosion = new Explosion(new ExplosionType3State());
			break;

			case ENEMY_TYPE::HUMAN:
				 Sound::GetInstance()->Play("qexplode", false, 1);
				 if (deadEntity->IsDrown())
				 {
					 explosion = new Explosion(new ExplosionDrownState());
				 }
				 else
				 {
					 explosion = new Explosion(new ExplosionType1State());
				 }
			break;

			case ENEMY_TYPE::MACHINE:
				 Sound::GetInstance()->Play("exbullet", false, 1);
				 explosion = new Explosion(new ExplosionType2State());
			break;

			}
			if (explosion)
			{
				explosion->SetX(deadEntity->GetX());
				explosion->SetY(deadEntity->GetY());
				if (deadEntity->IsDrown()) explosion->SetY(explosion->GetY() - Constants::Stages::EXPLOSION_SPAWN_OFFSET_Y);
				this->_effectEntities.push_back(explosion);
				this->ProcessSpecialExplosion(deadEntity);
			}
		}
		else
		if (deadEntity->IsBullet())
		{
			Bullet* bullet = static_cast<Bullet*>(deadEntity);
			if (this->ProcessSpecialBullet(bullet))
			{

			}
			else
			{
				Explosion* specialExplosion = bullet->CreateDeathExplosion();
				if (specialExplosion)
				{
					this->_effectEntities.push_back(specialExplosion);
				}
				else
				{
					this->_effectEntities.push_back(bullet->CreateBulletExplosion());
				}
			}
		}
		if (Item* item = deadEntity->CreateDroppedItem())
		{
			this->_entities->Insert(item);
		}
		this->_entitiesResult.erase(deadEntity);
		Destroy             (deadEntity);
	}
	for (auto& outOfBoundBullet : outOfBoundBullets)
	{
		this->_entitiesResult.erase(outOfBoundBullet);
		Destroy             (outOfBoundBullet);
	}
	for (auto& effectEntity : this->_effectEntities)
	{
		if (effectEntity->IsDead())
		{
			Destroy(effectEntity);
		}
		else
		{
			effectEntity->Update();
		}
	}
	std::erase_if(this->_effectEntities, [](Entity* effectEntity) -> bool { return effectEntity == nullptr; });


	if (QuadTreeNode::Update(this->_entities, this->_entitiesResult))
	{
		this->_entitiesResult.clear();
		this->_entities->Retrieve(this->_camera, this->_entitiesResult);
	}


	this->TranslateCamera();
	this->TranslateWalls ();
	this->CheckIfHasDone ();
	BulletParticleSystem::Update();

	this->_hasUpdated = true;
}


void Stage::Render()
{
	this->_backgroundTerrainsResult.clear();
	this->_backgroundTerrains->Retrieve(this->_camera, this->_backgroundTerrainsResult);
	for (auto& [backgroundTerrain, node] : this->_backgroundTerrainsResult) backgroundTerrain->Render();


	if (this->_hasDone)
	{
		this->RenderBossCompletion();
		return;
	}


	for (auto& [entity, node] : this->_entitiesResult) entity->Render();
	for (auto& effectEntity : this->_effectEntities) effectEntity->Render();
	this->_bill->Render();
	BulletParticleSystem::Render();
}


void Stage::HandleInput(Input& input)
{
	this->_bill->HandleInput(input);
}


void Stage::CheckResolveClearCollision()
{
	if (this->_hasDone)
		return;

	// Nothing to resolve against until Update has built the world once.  The game
	// loop calls this whenever Scene::stageIsReady is set, and the scene turns
	// that on from PlayingSceneState::Enter - which now runs inside Scene::Update,
	// on a step where the stage itself was still loading and so was never updated.
	// Resolving then would sweep Bill against the boundary walls while they still
	// sit at the origin where the stage constructor left them, and the WALL branch
	// of Bill::DynamicResolveOnCollision kills him for it.
	if (!this->_hasUpdated)
		return;

	this->_foregroundTerrainsResult.clear();
	this->_foregroundTerrains->Retrieve(this->_camera, this->_foregroundTerrainsResult);

	std::vector<Entity*> terrainCandidates;
	terrainCandidates.reserve(Constants::Stages::DEFAULT_CANDIDATE_RESERVE_CAPACITY);

	// 1. Entity vs Foreground Terrains accelerated by TLAS swept broadphase
	for (auto& [entity, node] : this->_entitiesResult)
	{
		if (auto collidableEntity = entity->AsCollidable())
		{
			terrainCandidates.clear();
			const Space::AABB sweptBox = Space::AABB::FromEntity(entity)
				.Swept(entity->GetVX(), entity->GetVY())
				.Expand(Constants::Physics::CANDIDATE_EXPAND_EPSILON);

			this->_foregroundTerrains->GetTLAS()->QueryAABB(sweptBox, [&](Entity* terrain) {
				terrainCandidates.push_back(terrain);
			});

			if (!terrainCandidates.empty())
			{
				for (auto* foregroundTerrain : terrainCandidates)
				{
					collidableEntity->CollideWith(foregroundTerrain);
				}
			}
			else
			{
				collidableEntity->DynamicResolveNoCollision();
			}
		}
	}

	// 2. Entity vs Entity accelerated by TLAS + BLAS broadphase
	std::vector<Entity*> entityCandidates;
	entityCandidates.reserve(Constants::Stages::DEFAULT_CANDIDATE_RESERVE_CAPACITY);

	for (auto& [entity1, node1] : this->_entitiesResult)
	{
		if (auto collidableEntity = entity1->AsCollidable())
		{
			entityCandidates.clear();
			const Space::AABB sweptBox1 = Space::AABB::FromEntity(entity1)
				.Swept(entity1->GetVX(), entity1->GetVY())
				.Expand(Constants::Physics::CANDIDATE_EXPAND_EPSILON);

			this->_entities->GetTLAS()->QueryAABB(sweptBox1, [&](Entity* candidate) {
				if (candidate && candidate != entity1)
				{
					entityCandidates.push_back(candidate);
				}
			});

			for (auto* entity2 : entityCandidates)
			{
				entity2->ForEachCollisionEntity([collidableEntity, &sweptBox1](Entity* subEntity) {
					if (Space::AABB::FromEntity(subEntity).Intersects(sweptBox1))
					{
						collidableEntity->CollideWith(subEntity);
					}
				});
			}

			if (entity1->CollidesWithBoundaryWalls())
			{
				for (auto& [name, wall] : this->_walls)
				{
					collidableEntity->CollideWith(wall);
				}
			}
		}
	}

	if (this->_bill)
	{
		for (auto& [name, wall] : this->_walls)
		{
			this->_bill->CollideWith(wall);
		}
	}
}


void  Stage::SetBill(Bill* bill) {		  this->_bill = bill; }
Bill* Stage::GetBill(          ) { return this->_bill       ; }


void    Stage::SetCamera(Camera* camera) {		  this->_camera = camera; }
Camera* Stage::GetCamera(              ) { return this->_camera         ; }


template void Stage::Load<TerrainStage1, CameraMovingForwardState>();
template void Stage::Load<TerrainStage2, CameraMovingUpwardState >();
template <class T, class S>
void Stage::Load()
{
	tson::Tileson tileson;
	std::unique_ptr<tson::Map> map = tileson.parse(fs::path(this->_mapFilePath));

	tson::Layer* backgroundTerrainsLayer = map.get()->getLayer("BackgroundLayer");
	tson::Layer* foregroundTerrainsLayer = map.get()->getLayer("ForegroundLayer");
	tson::Layer*           entitiesLayer = map.get()->getLayer("ObjectLayer");
	tson::Tileset& tileset = map.get()->getTilesets()[0];
	auto& tileSize = map.get()->getTileSize();
	auto&  mapSize = map.get()->getSize();

	this->_entities            = QuadTreeNode::New(0.0f, 0.0f, static_cast<float>(mapSize.x * tileSize.x), static_cast<float>(mapSize.y * tileSize.y));
	this->_backgroundTerrains  = QuadTreeNode::New(0.0f, 0.0f, static_cast<float>(mapSize.x * tileSize.x), static_cast<float>(mapSize.y * tileSize.y));
	this->_foregroundTerrains  = QuadTreeNode::New(0.0f, 0.0f, static_cast<float>(mapSize.x * tileSize.x), static_cast<float>(mapSize.y * tileSize.y));
	this->_tileW = static_cast<float>(tileSize.x);
	this->_tileH = static_cast<float>(tileSize.y);
	T::SetTileset(&tileset);

	if (!this->_camera) this->_camera = new Camera(new S());
	if (!this->_bill)   this->_bill = new Bill();

	this->LoadBackgroundTerrains<T>(backgroundTerrainsLayer);
	this->LoadForegroundTerrains   (foregroundTerrainsLayer);
	this->LoadEntities(entitiesLayer);
	this->_entities->Insert(this->_bill);

	// `tileset` is a reference into `map`, which dies with this scope.
	T::SetTileset(nullptr);
}


template void Stage::LoadBackgroundTerrains<TerrainStage1>(void*);
template void Stage::LoadBackgroundTerrains<TerrainStage2>(void*);
template <class T>
void Stage::LoadBackgroundTerrains(void* backgroundTerrainsLayer)
{
	auto backgroundLayer = static_cast<tson::Layer*>(backgroundTerrainsLayer);
	auto& tileObjects = backgroundLayer->getTileObjects();
	std::vector<Entity*> backgroundTerrainList;
	backgroundTerrainList.reserve(tileObjects.size());

	for (auto& [tileObjectPosition, tileObject] : tileObjects)
	{
		T* backgroundTerrain = new T();
		auto animationId = std::to_string(tileObject.getTile()->getId()); auto& position = tileObject.getPosition(); auto& size = tileObject.getTile()->getTileSize();

		backgroundTerrain->SetW(static_cast<float>(size.x));
		backgroundTerrain->SetH(static_cast<float>(size.y));
		backgroundTerrain->SetAnimationId(animationId);
		backgroundTerrain->SetX(position.x + size.x * 0.5f);
		backgroundTerrain->SetY(position.y + size.y * 0.0f);

		backgroundTerrainList.push_back(backgroundTerrain);
	}

	for (size_t i = 0; i < backgroundTerrainList.size() / 2; i++)
	{
		auto y1 = backgroundTerrainList[i]->GetY();
		auto y2 = backgroundTerrainList[backgroundTerrainList.size() - 1 - i]->GetY();
		std::swap(y1, y2);
		backgroundTerrainList[i]->SetY(y1);
		backgroundTerrainList[backgroundTerrainList.size() - 1 - i]->SetY(y2);
	}
	std::sort(backgroundTerrainList.begin(), backgroundTerrainList.end(), [](Entity* e1, Entity* e2) -> bool { return e1->GetX() < e2->GetX(); });

	if (!backgroundTerrainList.empty())
	{
		auto representativeBackgroundTerrain = static_cast<T*>(backgroundTerrainList.front());
		if (representativeBackgroundTerrain)
		{
			representativeBackgroundTerrain->LoadTextures  ();
			representativeBackgroundTerrain->LoadSprites   ();
			representativeBackgroundTerrain->LoadAnimations();
		}
	}

	for (auto& bgTerrain : backgroundTerrainList)
		this->_backgroundTerrains->Insert(bgTerrain);
	backgroundTerrainList.clear();
}


void Stage::LoadForegroundTerrains(void* foregroundTerrainsLayer)
{
	auto foregroundLayer = static_cast<tson::Layer*>(foregroundTerrainsLayer);
	auto mapH = foregroundLayer->getMap()->getSize    ().y
		      * foregroundLayer->getMap()->getTileSize().y;

	for (auto& object : foregroundLayer->getObjects())
	{
		TerrainBlock* foregroundTerrain = new TerrainBlock();
		auto& position = object.getPosition();
		auto& size     = object.getSize    ();

		foregroundTerrain->SetX(	   position.x + size.x * 0.5f);
		foregroundTerrain->SetY(mapH - position.y - size.y * 1.0f);
		foregroundTerrain->SetW(static_cast<float>(size.x));
		foregroundTerrain->SetH(static_cast<float>(size.y));

		if (object.getClassType() == "non_throughable")
		foregroundTerrain->SetTerrainType(TERRAIN_BLOCK_TYPE::NON_THROUGHABLE);
		if (object.getClassType() == "throughable")
		foregroundTerrain->SetTerrainType(TERRAIN_BLOCK_TYPE::THROUGHABLE);
		if (object.getClassType() == "check_point")
		foregroundTerrain->SetTerrainType(TERRAIN_BLOCK_TYPE::CHECK_POINT);
		if (object.getClassType() == "water")
		foregroundTerrain->SetTerrainType(TERRAIN_BLOCK_TYPE::WATER);

		this->_foregroundTerrains->Insert(foregroundTerrain);

		if (foregroundTerrain
		&&  foregroundTerrain->GetTerrainType() == TERRAIN_BLOCK_TYPE::CHECK_POINT)
			this->_checkPoint = foregroundTerrain;
	}
}


