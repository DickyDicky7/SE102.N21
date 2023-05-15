#pragma once

#include "Common.h"

class Bill; class Input; class Entity;

class Stage
{

public:

	Stage();
	virtual ~Stage();
	virtual void Load() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void HandleInput(Input&) = 0;

protected:

	Bill* bill;
	std::list<Entity*> entities;		   // SOLDIER, WALL TURRET, ETC.
	std::list<Entity*> backgroundTerrains; // TERRAIN STAGE 1 OR 2 OR 3
	std::list<Entity*> collidableTerrains; // TERRAIN BLOCK

	virtual void LoadMap() = 0;
	virtual void LoadEntities(void*) = 0;
	virtual void LoadBackgroundTerrains(void*) = 0;
	virtual void LoadCollidableTerrains(void*) = 0;

	virtual void RenderMap() = 0;
	virtual void RenderEntities() = 0;
	virtual void RenderBackgroundTerrains() = 0;
	virtual void RenderCollidableTerrains() = 0;

};

