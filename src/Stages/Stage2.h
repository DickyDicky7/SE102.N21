#pragma once

#include "Stage.h"

class BossStage3; class BossStage3Hand;

class Stage2 : public Stage
{

public:

	Stage2();
	virtual ~Stage2();
	virtual void CheckIfHasDone() override;

protected:

	// The head reads both hands every Update (IsHandsDead) and the gate reads the
	// head (IsHeadDead), so these are kept alive after they die - in
	// _deadBossParts, freed with the stage - instead of being destroyed by
	// Stage::Update while those pointers are still in use.
	BossStage3*     _bossStage3Head;
	BossStage3Hand* _bossStage3HandLeft;
	BossStage3Hand* _bossStage3HandRight;
	std::vector<Entity*> _deadBossParts;

	virtual bool RetainDeadEntity(Entity* deadEntity) override;

	virtual void TranslateWalls () override;
	virtual void TranslateCamera() override;
	virtual void SetRevivalPoint() override;
	virtual bool ProcessSpecialEntity   (Entity* entity) override;
	virtual bool ProcessSpecialBullet   (Bullet* bullet) override;
	virtual bool ProcessSpecialExplosion(Entity* deadEntity) override;

	virtual void LoadEntities(void* entitiesLayer) override;

};

