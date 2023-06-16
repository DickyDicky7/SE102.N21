#pragma once

#include "Common.h"
#include "Entity.h"
#include "QuadTreeNode.h"

class Bill; class Input; class Camera;

class Stage
{

public:

	Stage();
	virtual ~Stage();
	virtual void Update();
	virtual void Render();
	virtual void HandleInput(Input&);
	template <class T, class S> void Load();
	virtual void CheckResolveClearCollision();

	virtual void  SetBill(Bill*);
	virtual Bill* GetBill(     );

	virtual void    SetCamera(Camera*);
	virtual Camera* GetCamera(       );

protected:

	std::string mapFilePath;
	Bill* bill;
	FLOAT tileW;
	FLOAT tileH;
	Camera* camera;
	QuadTreeNode* entities;
	QuadTreeNode* backgroundTerrains;
	QuadTreeNode* foregroundTerrains;
	std::list<Entity*> effectEntities;
	std::unordered_map<std::string, Entity*> walls;
	std::unordered_map<Entity*, QuadTreeNode*> entitiesResult;
	std::unordered_map<Entity*, QuadTreeNode*> backgroundTerrainsResult;
	std::unordered_map<Entity*, QuadTreeNode*> foregroundTerrainsResult;

	virtual void LoadEntities(void*) = 0;
	template <class T> void LoadBackgroundTerrains(void*);
				       void LoadForegroundTerrains(void*);

};


