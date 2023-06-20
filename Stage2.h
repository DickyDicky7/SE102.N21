#pragma once

#include "Stage.h"

class Stage2 : public Stage
{

public:

	Stage2();
	virtual ~Stage2();

protected:

	virtual void TranslateWalls () override;
	virtual void TranslateCamera() override;
	virtual void SetRevivalPoint() override;

	virtual void LoadEntities(void*) override;

};

