#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"

class RockFall;
class RockFallState;
class RockFallNormalState;
class RockFallFallState;


class RockFall : public Entity, public Enemy<Bill>
	, public HasTextures<RockFall>, public HasSprites<RockFall>, public HasAnimations<RockFall>
{
public:
	RockFall();
	virtual ~RockFall();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void setTimeDelayToFall(FLOAT time)
	{
		_timedelayToFall = time;
	}
	FLOAT getTimeDelayToFall() 
	{
		return _timedelayToFall;
	}
protected:
	RockFallState* state;
	RockFallState* updateState;
	RockFallState* handleInputState;

	FLOAT _timedelayToFall;
};



class RockFallState : public State<RockFallState, RockFall>
{

public:

	RockFallState();
	virtual ~RockFallState();

	virtual void Exit(RockFall&) override = 0;
	virtual void Enter(RockFall&) override = 0;
	virtual void Render(RockFall&) override = 0;

	virtual RockFallState* Update(RockFall&) override = 0;
	virtual RockFallState* HandleInput(RockFall&, Input&) override = 0;
};


class RockFallNormalState : public RockFallState
{

public:

	RockFallNormalState();
	virtual ~RockFallNormalState();

	virtual void Exit(RockFall&) override;
	virtual void Enter(RockFall&) override;
	virtual void Render(RockFall&) override;

	virtual RockFallState* Update(RockFall&) override;
	virtual RockFallState* HandleInput(RockFall&, Input&) override;
};


class RockFallFallState : public RockFallState
{

public:

	RockFallFallState();
	virtual ~RockFallFallState();

	virtual void Exit(RockFall&) override;
	virtual void Enter(RockFall&) override;
	virtual void Render(RockFall&) override;

	virtual RockFallState* Update(RockFall&) override;
	virtual RockFallState* HandleInput(RockFall&, Input&) override;
};