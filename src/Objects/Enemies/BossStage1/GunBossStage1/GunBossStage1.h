#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"
#include "BossStage1.h"
#include "ExplosionCommon.h"
#include "HasWeapons.h"

class GunBossStage1;
class GunBossStage1State;
class GunBossStage1NormalState;
class GunBossStage1PrepareShootState;
class GunBossStage1DestroyState;

class GunBossStage1 : public Entity, public Enemy<Bill>
	, public HasTextures<GunBossStage1>, public HasSprites<GunBossStage1>, public HasAnimations<GunBossStage1>, public BossStage1, public HasWeapons
{
public:
	GunBossStage1();
	GunBossStage1(int type);
	~GunBossStage1();

	virtual void Update() override;
	virtual void Render() override;
	virtual void HandleInput(Input& input) override;

	void Fire() override;
	void Fire(float angle, float vx, float vy, float ax, float ay, DIRECTION direction);

	void LoadTextures() override;
	void LoadSprites() override;
	void LoadAnimations() override;
	int GetType() const;

	const Bill* GetEnemyTarget();

	bool IsEnemy() const override { return true; }
	bool ShouldRetainWhenDead() const override { return true; }
	ENEMY_TYPE GetEnemyType() const override { return this->_enemyType; }
	bool IsPushableObstacle() const override { return !this->IsDead(); }
	bool IsVulnerableToBullet() const override { return !this->IsDead(); }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
	bool TakeBulletHit() override { return this->IsDead() ? false : this->Enemy<Bill>::TakeEnemyBulletHit(this); }
protected:
	GunBossStage1State* _state;
	GunBossStage1State* _updateState;
	int _type;
};

class GunBossStage1State : public State<GunBossStage1State, GunBossStage1>
{
public:
	GunBossStage1State();
	~GunBossStage1State();

	virtual void Exit(GunBossStage1& gunBossStage1) = 0;
	virtual void Enter(GunBossStage1& gunBossStage1) = 0;
	virtual void Render(GunBossStage1& gunBossStage1) = 0;

	virtual GunBossStage1State* Update(GunBossStage1& gunBossStage1) = 0;
	virtual GunBossStage1State* HandleInput(GunBossStage1& gunBossStage1, Input& input) override;

protected:
	float _time;
};

class GunBossStage1NormalState : public GunBossStage1State
{
public:
	GunBossStage1NormalState();
	GunBossStage1NormalState(float time);
	~GunBossStage1NormalState();

	virtual void Exit(GunBossStage1& gunBossStage1);
	virtual void Enter(GunBossStage1& gunBossStage1);
	virtual void Render(GunBossStage1& gunBossStage1);

	virtual GunBossStage1State* Update(GunBossStage1& gunBossStage1);

};

class GunBossStage1PrepareShootState : public GunBossStage1State
{
public:
	GunBossStage1PrepareShootState();
	GunBossStage1PrepareShootState(float time);
	~GunBossStage1PrepareShootState();

	virtual void Exit(GunBossStage1& gunBossStage1);
	virtual void Enter(GunBossStage1& gunBossStage1);
	virtual void Render(GunBossStage1& gunBossStage1);

	virtual GunBossStage1State* Update(GunBossStage1& gunBossStage1);
};


class GunBossStage1DestroyState : public GunBossStage1State
{
public:
	GunBossStage1DestroyState();
	~GunBossStage1DestroyState();

	virtual void Exit(GunBossStage1& gunBossStage1);
	virtual void Enter(GunBossStage1& gunBossStage1);
	virtual void Render(GunBossStage1& gunBossStage1);

	virtual GunBossStage1State* Update(GunBossStage1& gunBossStage1);

protected:
	bool _isDestroy;
	bool _isInDestroyPos;
};

