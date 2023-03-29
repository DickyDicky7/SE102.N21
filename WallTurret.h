#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"

class WallTurret;
class WallTurretState;
class WallTurretNormalState;
class WallTurretSwingRightUpState;
class WallTurretSwingRightDownState;
class WallTurretSwingLeftUpState;
class WallTurretSwingLeftDownState;


class WallTurret : public Entity<WallTurret>, public HasAnimations
{
public:
	WallTurret();
	virtual ~WallTurret();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override = 0;

protected:
	WallTurretState* state;
	WallTurretState* updateState;
	//WallTurretState* handleInputState;
};

class WallTurretState : public State<WallTurretState, WallTurret>
{
public:
	WallTurretState(DIRECTION);
	virtual ~WallTurretState();

	virtual void Exit(WallTurret&) override = 0;
	virtual void Enter(WallTurret&) override = 0;
	virtual void Render(WallTurret&) override = 0;

	virtual WallTurretState* Update(WallTurret&) override = 0;
	virtual WallTurretNormalState* HandleInput(WallTurret&, Input&) override = 0;
};

class WallTurretNormalState : WallTurretState
{
	WallTurretNormalState(DIRECTION);
	virtual ~WallTurretNormalState();

	virtual void Exit(WallTurret&) override = 0;
	virtual void Enter(WallTurret&) override = 0;
	virtual void Render(WallTurret&) override = 0;

	virtual WallTurretNormalState* Update(WallTurret&) override = 0;
	virtual WallTurretNormalState* HandleInput(WallTurret&, Input&) override = 0;

};