#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"
#include "BossStage3Hand.h"
#include "Sound.h"

class BossStage3;
class BossStage3State;
class BossStage3OpenState;
class BossStage3CloseState;
class BossStage3MiddleState;


class BossStage3 : public Entity, public Enemy<Bill>
				 , public HasTextures<BossStage3>, public HasSprites<BossStage3>, public HasAnimations<BossStage3>, public HasWeapons
{
public:
	BossStage3();
	virtual ~BossStage3();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void  Fire() override;

	bool GetIsFire() const;
	void SetIsFire(bool check);

	BossStage3Hand* GetHandLeft() const;
	BossStage3Hand* GetHandRight() const;
	void SetHandLeft(BossStage3Hand* handLeft);
	void SetHandRight(BossStage3Hand* handRight);
	bool IsHandsDead() const;

	bool IsEnemy() const override;
	ENEMY_TYPE GetEnemyType() const override;
	void SetTarget(const Bill* target) override;
	void ProcessSpecialDeathEffects(std::vector<Entity*>& effectEntities) override;
	bool TakeBulletHit() override;
protected:
	BossStage3State* _state;
	BossStage3State* _updateState;
	BossStage3State* _handleInputState;

	BossStage3Hand* _boss3Stage3HandLeft;
	BossStage3Hand* _boss3Stage3HandRight;
	bool _isCounted;
	bool _isFire;
};



class BossStage3State : public State<BossStage3State, BossStage3>
{

public:

	BossStage3State();
	virtual ~BossStage3State();

	virtual void Exit(BossStage3& bossStage3) override = 0;
	virtual void Enter(BossStage3& bossStage3) override = 0;
	virtual void Render(BossStage3& bossStage3) override = 0;

	virtual BossStage3State* Update(BossStage3& bossStage3) override = 0;
	virtual BossStage3State* HandleInput(BossStage3& bossStage3, Input& input) override = 0;

	virtual bool CanFire() const;

protected:
	float _time;
};


class BossStage3OpenState : public BossStage3State
{

public:

	BossStage3OpenState();
	virtual ~BossStage3OpenState();

	virtual void Exit(BossStage3& bossStage3) override;
	virtual void Enter(BossStage3& bossStage3) override;
	virtual void Render(BossStage3& bossStage3) override;

	virtual BossStage3State* Update(BossStage3& bossStage3) override;
	virtual BossStage3State* HandleInput(BossStage3& bossStage3, Input& input) override;

	bool CanFire() const override;
};


class BossStage3CloseState : public BossStage3State
{

public:

	BossStage3CloseState();
	virtual ~BossStage3CloseState();

	virtual void Exit(BossStage3& bossStage3) override;
	virtual void Enter(BossStage3& bossStage3) override;
	virtual void Render(BossStage3& bossStage3) override;

	virtual BossStage3State* Update(BossStage3& bossStage3) override;
	virtual BossStage3State* HandleInput(BossStage3& bossStage3, Input& input) override;
};

class BossStage3MiddleState : public BossStage3State
{

public:

	BossStage3MiddleState(BOSS_STAGE_3_ANIMATION_ID nextState);
	virtual ~BossStage3MiddleState();

	virtual void Exit(BossStage3& bossStage3) override;
	virtual void Enter(BossStage3& bossStage3) override;
	virtual void Render(BossStage3& bossStage3) override;

	virtual BossStage3State* Update(BossStage3& bossStage3) override;
	virtual BossStage3State* HandleInput(BossStage3& bossStage3, Input& input) override;
protected:
	TIME _delayTime;
	BOSS_STAGE_3_ANIMATION_ID _nextState;
};
