#pragma once

#include "Stage.h"

class Stage1 : public Stage
{

public:

	Stage1();
	virtual ~Stage1();
	virtual void CheckIfHasDone() override;

protected:

	virtual void TranslateWalls () override;
	virtual void TranslateCamera() override;
	virtual void SetRevivalPoint() override;

	virtual void LoadEntities(void*) override;

};

