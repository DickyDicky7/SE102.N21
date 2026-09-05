#pragma once

#include "Stage.h"

class Stage2 : public Stage
{

public:

	Stage2();
	virtual ~Stage2();
	virtual void CheckIfHasDone() override;

protected:

	virtual void TranslateWalls () override;
	virtual void TranslateCamera() override;
	virtual void SetRevivalPoint() override;
	virtual bool ProcessSpecialEntity   (Entity* entity) override;
	virtual bool ProcessSpecialBullet   (Bullet* bullet) override;
	virtual bool ProcessSpecialExplosion(Entity* deadEntity) override;

	virtual void LoadEntities(void* entitiesLayer) override;

};

