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

class Falcon;
class FalconState;
class FalconCloseState;
class FalconOpeningState;
class FalconOpenState;


class Falcon : public Entity, public Enemy<Falcon, Bill>
	, public HasTextures<Falcon>, public HasSprites<Falcon>, public HasAnimations<Falcon>
{
public:
	Falcon();
	virtual ~Falcon();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void CalculateBillDistance();

	void SetCurrentState(FALCON_ANIMATION_ID id)
	{
		currentState = id;
	}

	FALCON_ANIMATION_ID getCurrentState()
	{
		return currentState;
	}

protected:
	FalconState* state;
	FalconState* updateState;
	FalconState* handleInputState;

	FLOAT billDistance;
	FALCON_ANIMATION_ID currentState;
};



class FalconState : public State<FalconState, Falcon>
{

public:

	FalconState();
	virtual ~FalconState();

	virtual void Exit(Falcon&) override = 0;
	virtual void Enter(Falcon&) override = 0;
	virtual void Render(Falcon&) override = 0;

	virtual FalconState* Update(Falcon&) override = 0;
	virtual FalconState* HandleInput(Falcon&, Input&) override = 0;
};


class FalconCloseState : public FalconState
{

public:

	FalconCloseState();
	virtual ~FalconCloseState();

	virtual void Exit(Falcon&) override;
	virtual void Enter(Falcon&) override;
	virtual void Render(Falcon&) override;

	virtual FalconState* Update(Falcon&) override;
	virtual FalconState* HandleInput(Falcon&, Input&) override;
};


class FalconOpeningState : public FalconState
{

public:

	FalconOpeningState(FALCON_ANIMATION_ID nextState);
	virtual ~FalconOpeningState();

	virtual void Exit(Falcon&) override;
	virtual void Enter(Falcon&) override;
	virtual void Render(Falcon&) override;

	virtual FalconState* Update(Falcon&) override;
	virtual FalconState* HandleInput(Falcon&, Input&) override;
protected:
	TIME delayTime;
	FALCON_ANIMATION_ID nextState;
};

class FalconOpenState : public FalconState
{

public:

	FalconOpenState();
	virtual ~FalconOpenState();

	virtual void Exit(Falcon&) override;
	virtual void Enter(Falcon&) override;
	virtual void Render(Falcon&) override;

	virtual FalconState* Update(Falcon&) override;
	virtual FalconState* HandleInput(Falcon&, Input&) override;
};