#pragma once

#include "Common.h"

class Bill; class Input; class Entity; class Camera; class QuadTreeNode;

class Stage
{

public:

	Stage();
	virtual ~Stage();
	virtual void Load() = 0;
	virtual void Update();
	virtual void Render();
	virtual void HandleInput(Input&);
	virtual void CheckResolveClearCollision();

	virtual void  SetBill(Bill*);
	virtual Bill* GetBill(     );

	virtual void    SetCamera(Camera*);
	virtual Camera* GetCamera(       );

protected:

	Bill* bill;
	FLOAT tileW;
	FLOAT tileH;
	Camera* camera;
	QuadTreeNode* entities;
	QuadTreeNode* backgroundTerrains;
	QuadTreeNode* foregroundTerrains;
	std::unordered_set<Entity*> _entities;
	std::unordered_set<Entity*> _backgroundTerrains;
	std::unordered_set<Entity*> _foregroundTerrains;

	virtual void LoadEntities(void*) = 0;
	virtual void LoadBackgroundTerrains(void*) = 0;
	virtual void LoadForegroundTerrains(void*) = 0;

	virtual BOOL IsLORPointOnTheScreenByX(Entity*);
	virtual BOOL IsTOBPointOnTheScreenByY(Entity*);

};
