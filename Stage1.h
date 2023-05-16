#pragma once

#include "Stage.h"

class Stage1 : public Stage
{

public:

	Stage1();
	virtual ~Stage1();
	virtual void Load() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput(Input&) override;

protected:

	virtual void LoadMap() override;
	virtual void LoadEntities(void*) override;
	virtual void LoadBackgroundTerrains(void*) override;
	virtual void LoadCollidableTerrains(void*) override;

	virtual void RenderMap() override;
	virtual void RenderEntities() override;
	virtual void RenderBackgroundTerrains() override;
	virtual void RenderCollidableTerrains() override;

};
