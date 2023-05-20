#pragma once

#include "Stage.h"

class Stage1 : public Stage
{

public:

	Stage1();
	virtual ~Stage1();
	virtual void Load() override;

protected:

	virtual void LoadEntities(void*) override;
	virtual void LoadBackgroundTerrains(void*) override;
	virtual void LoadForegroundTerrains(void*) override;

};
