#pragma once

#include "Common.h"
#include "Entity.h"
#include "QuadTreeNode.h"

class Bill; class Input; class Camera; class Bullet; class TerrainBlock;

class Stage
{

public:

	Stage();
	virtual ~Stage();
	virtual void Update();
	virtual void Render();
	virtual void HandleInput(Input& input);
	virtual void RenderBossCompletion();
	template <class T, class S> void Load();
	virtual void CheckResolveClearCollision();

	template <class T, typename... Args>
	static void PreloadResources(Args&&... args)
	{
		T representative(std::forward<Args>(args)...);
		representative.LoadTextures();
		representative.LoadSprites();
		representative.LoadAnimations();
	}

	virtual void  SetBill(Bill* bill);
	virtual Bill* GetBill(          );

	bool HasDone() const;
	void SetHasDone(bool done = true);
	virtual void  CheckIfHasDone() = 0;

	virtual void    SetCamera(Camera* camera);
	virtual Camera* GetCamera(              );

protected:

	bool _hasDone;
	TerrainBlock* _checkPoint;

	// Cleared until Update has run once.  CheckResolveClearCollision needs the
	// world Update builds - the walls positioned by TranslateWalls, and the
	// camera-visible set in entitiesResult - and gets called on its own from the
	// game loop, so it has to be able to tell that Update has not happened yet.
	bool _hasUpdated;

	std::string _mapFilePath;
	float _translateX;
	float _translateY;
	Bill* _bill;
	float _tileW;
	float _tileH;
	Camera* _camera;
	QuadTreeNode* _entities;
	QuadTreeNode* _backgroundTerrains;
	QuadTreeNode* _foregroundTerrains;
	std::vector<Entity*> _effectEntities;
	std::unordered_map<std::string, Entity*> _walls;
	std::unordered_map<Entity*, QuadTreeNode*> _entitiesResult;
	std::unordered_map<Entity*, QuadTreeNode*> _backgroundTerrainsResult;
	std::unordered_map<Entity*, QuadTreeNode*> _foregroundTerrainsResult;

	virtual void TranslateWalls () = 0;
	virtual void TranslateCamera() = 0;
	virtual void SetRevivalPoint() = 0;
	virtual bool ProcessSpecialEntity   (Entity* entity) = 0;
	virtual bool ProcessSpecialBullet   (Bullet* bullet) = 0;
	virtual bool ProcessSpecialExplosion(Entity* deadEntity) = 0;
	// Called for a dead entity once it is out of the tree and its explosion has
	// been spawned.  Returning true hands ownership to the stage instead of the
	// entity being destroyed there and then - for entities other entities still
	// hold raw pointers to and read every Update.
	virtual bool RetainDeadEntity       (Entity* deadEntity);

	virtual void LoadEntities(void* entitiesLayer) = 0;
	template <class T> void LoadBackgroundTerrains(void* backgroundTerrainsLayer);
				       void LoadForegroundTerrains(void* foregroundTerrainsLayer);

};


