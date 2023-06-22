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
	virtual BOOL ProcessSpecialEntity   (Entity*) override;
	virtual BOOL ProcessSpecialBullet   (Bullet*) override;
	virtual BOOL ProcessSpecialExplosion(Entity*) override;

	virtual void LoadEntities(void*) override;

};

