#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"
#include "AirCraft.h"
#include "ItemCommon.h"

class Falcon;
class FalconState;
class FalconCloseState;
class FalconOpeningState;
class FalconOpenState;


class Falcon : public Entity, public Enemy<Bill>
	, public HasTextures<Falcon>, public HasSprites<Falcon>, public HasAnimations<Falcon>
{
public:
	Falcon(ITEM_TYPE ammoType);
	virtual ~Falcon();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void CalculateBillDistance();

	void SetCurrentState(FALCON_ANIMATION_ID id)
	{
		this->_currentState = id;
	}

	FALCON_ANIMATION_ID GetCurrentState()
	{
		return this->_currentState;
	}

	ITEM_TYPE GetAmmoType();
	void SetAmmoType(ITEM_TYPE ammoType);

	bool IsEnemy() const override { return true; }
	ENEMY_TYPE GetEnemyType() const override { return this->_enemyType; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
	Item* CreateDroppedItem() const override;
	bool TakeBulletHit() override { return this->Enemy<Bill>::TakeEnemyBulletHit(this); }
protected:
	FalconState* _state;
	FalconState* _updateState;
	FalconState* _handleInputState;

	ITEM_TYPE _ammoType;
	float _billDistance;
	FALCON_ANIMATION_ID _currentState;
};



class FalconState : public State<FalconState, Falcon>
{

public:

	FalconState();
	virtual ~FalconState();

	virtual void Exit(Falcon& falcon) override = 0;
	virtual void Enter(Falcon& falcon) override = 0;
	virtual void Render(Falcon& falcon) override = 0;

	virtual FalconState* Update(Falcon& falcon) override = 0;
	virtual FalconState* HandleInput(Falcon& falcon, Input& input) override = 0;
};


class FalconCloseState : public FalconState
{

public:

	FalconCloseState();
	virtual ~FalconCloseState();

	virtual void Exit(Falcon& falcon) override;
	virtual void Enter(Falcon& falcon) override;
	virtual void Render(Falcon& falcon) override;

	virtual FalconState* Update(Falcon& falcon) override;
	virtual FalconState* HandleInput(Falcon& falcon, Input& input) override;
};


class FalconOpeningState : public FalconState
{

public:

	FalconOpeningState(FALCON_ANIMATION_ID nextState);
	virtual ~FalconOpeningState();

	virtual void Exit(Falcon& falcon) override;
	virtual void Enter(Falcon& falcon) override;
	virtual void Render(Falcon& falcon) override;

	virtual FalconState* Update(Falcon& falcon) override;
	virtual FalconState* HandleInput(Falcon& falcon, Input& input) override;
protected:
	TIME _delayTime;
	FALCON_ANIMATION_ID _nextState;
};

class FalconOpenState : public FalconState
{

public:

	FalconOpenState();
	virtual ~FalconOpenState();

	virtual void Exit(Falcon& falcon) override;
	virtual void Enter(Falcon& falcon) override;
	virtual void Render(Falcon& falcon) override;

	virtual FalconState* Update(Falcon& falcon) override;
	virtual FalconState* HandleInput(Falcon& falcon, Input& input) override;
};
