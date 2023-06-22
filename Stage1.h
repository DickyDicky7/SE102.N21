#pragma once

#include "Stage.h"

class FinalBossStage1;

class Stage1 : public Stage
{

public:

	FinalBossStage1* finalBossStage1;

	Stage1();
	virtual ~Stage1();
	virtual void CheckIfHasDone() override;

protected:

	virtual void TranslateWalls () override;
	virtual void TranslateCamera() override;
	virtual void SetRevivalPoint() override;

	virtual void LoadEntities(void*) override;

};

