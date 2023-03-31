#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"

class WallTurret;
class WallTurretState;

// basic
class WallTurretLeftState;
class WallTurretRightState;
class WallTurretUpState;
class WallTurretDownState;

//angle right side of circle
class WallTurretRightUp30State;
class WallTurretRightUp60State;

class WallTurretRightDown30State;
class WallTurretRightDown60State;

//angle left side of circle
class WallTurretLeftUp30State;
class WallTurretLeftUp60State;

class WallTurretLeftDown30State;
class WallTurretLeftDown60State;


class WallTurret : public Entity<WallTurret>, public HasAnimations
{
public:
	WallTurret();
	virtual ~WallTurret();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;
	void LoadSprite();

protected:
	WallTurretState* state;
	WallTurretState* updateState;
};

class WallTurretState : public State<WallTurretState, WallTurret>
{
public:
	WallTurretState();
	virtual ~WallTurretState();

	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;
	virtual void Render(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
	virtual WallTurretState* HandleInput(WallTurret&, Input&) override;
};

class WallTurretLeftState : public WallTurretState
{
public:
	WallTurretLeftState();
	virtual ~WallTurretLeftState();

	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;
	virtual void Render(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretRightState : public WallTurretState
{
public:
	WallTurretRightState();
	virtual ~WallTurretRightState();

	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;
	virtual void Render(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretUpState : public WallTurretState
{
public:
	WallTurretUpState();
	virtual ~WallTurretUpState();

	virtual void Render(WallTurret&) override;
	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;
	
	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretDownState : public WallTurretState
{
public:
	WallTurretDownState();
	virtual ~WallTurretDownState();

	virtual void Render(WallTurret&) override;
	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretRightUp30State : public WallTurretState
{
public:
	WallTurretRightUp30State();
	virtual ~WallTurretRightUp30State();

	virtual void Render(WallTurret&) override;
	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretRightUp60State : public WallTurretState
{
public:
	WallTurretRightUp60State();
	virtual ~WallTurretRightUp60State();

	virtual void Render(WallTurret&) override;
	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretRightDown30State : public WallTurretState
{
public:
	WallTurretRightDown30State();
	virtual ~WallTurretRightDown30State();

	virtual void Render(WallTurret&) override;
	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretRightDown60State : public WallTurretState
{
public:
	WallTurretRightDown60State();
	virtual ~WallTurretRightDown60State();

	virtual void Render(WallTurret&) override;
	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretLeftUp30State : public WallTurretState
{
public:
	WallTurretLeftUp30State();
	virtual ~WallTurretLeftUp30State();

	virtual void Render(WallTurret&) override;
	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretLeftUp60State : public WallTurretState
{
public:
	WallTurretLeftUp60State();
	virtual ~WallTurretLeftUp60State();

	virtual void Render(WallTurret&) override;
	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretLeftDown30State : public WallTurretState
{
public:
	WallTurretLeftDown30State();
	virtual ~WallTurretLeftDown30State();

	virtual void Render(WallTurret&) override;
	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};

class WallTurretLeftDown60State : public WallTurretState
{
public:
	WallTurretLeftDown60State();
	virtual ~WallTurretLeftDown60State();

	virtual void Render(WallTurret&) override;
	virtual void Exit(WallTurret&) override;
	virtual void Enter(WallTurret&) override;

	virtual WallTurretState* Update(WallTurret&) override;
};