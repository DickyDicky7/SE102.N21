#pragma once

#include "Stage.h"

class FinalBossStage1;

class Stage1 : public Stage
{

public:

	Stage1();
	virtual ~Stage1();
	virtual void CheckIfHasDone() override;
	virtual void RenderBossCompletion() override;

protected:

	FinalBossStage1* _finalBossStage1;

	virtual void TranslateWalls () override;
	virtual void TranslateCamera() override;
	virtual void SetRevivalPoint() override;
	virtual bool ProcessSpecialEntity   (Entity* entity) override;
	virtual bool ProcessSpecialBullet   (Bullet* bullet) override;
	virtual bool ProcessSpecialExplosion(Entity* deadEntity) override;

	virtual void LoadEntities(void* entitiesLayer) override;

};

