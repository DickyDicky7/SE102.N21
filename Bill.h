#pragma once


#include "State.h"
#include "Motion.h"
#include "Common.h"
#include "Entity.h"
#include "HasWeapons.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"
#include "CollidableEntity.h"


class Bill;
class BillState;
class BillRunState;
class BillFallState;
class BillJumpState;
class BillDiveState;
class BillDeadState;
class BillBeginState;
class BillNormalState;
class BillLayDownState;
class BillRunShotState;
class BillSwimRunState;
class BillBeginSwimState;
class BillSwimNormalState;
class BillNormalShotState;
class BillStraightUpState;
class BillSwimRunShotState;
class BillSwimNormalShotState;
class BillShotStraightUpState;
class BillRunShotAngleUpState;
class BillSwimShotAngleUpState;
class BillRunShotAngleDownState;
class BillSwimShotStraightUpState;


class Bill : public Entity, public HasTextures<Bill>, public HasSprites<Bill>, public HasAnimations<Bill>, public CollidableEntity, public HasWeapons
{

public:

	INT* livesLeft;
	ULONGLONG immortalTime;
	ULONGLONG immortalTick;

	Bill();
	virtual ~Bill();
	virtual void GoDead();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void  Fire                    (               ) override;
	void  StaticResolveNoCollision(               ) override;
	void  StaticResolveOnCollision(AABBSweepResult) override;
	void DynamicResolveNoCollision(               ) override;
	void DynamicResolveOnCollision(AABBSweepResult) override;

protected:

	BillState* state;
	BillState* updateState;
	BillState* handleInputState;

};


class BillState : public State<BillState, Bill>
{

public:

	BillState();
	virtual ~BillState();

	virtual void Exit(Bill&) override = 0;
	virtual void Enter(Bill&) override = 0;
	virtual void Render(Bill&) override = 0;

	virtual BillState* Update(Bill&) override = 0;
	virtual BillState* HandleInput(Bill&, Input&) override = 0;

protected:

	FLOAT time;

};


class BillRunState : public BillState
{

public:

	BillRunState();
	virtual ~BillRunState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillFallState : public BillState
{

public:

	BillFallState();
	BillFallState(BillState*);
	virtual ~BillFallState( );

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

protected:

	BillState* returnState;

};


class BillJumpState : public BillState
{

public:

	BillJumpState();
	virtual ~BillJumpState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

protected:

	BOOL hasMovedLeft;
	BOOL hasMovedRight;

};


class BillDiveState : public BillState
{

public:

	BillDiveState();
	virtual ~BillDiveState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillDeadState : public BillState
{

public:

	BillDeadState();
	virtual ~BillDeadState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

protected:

	static constexpr ULONGLONG DEFAULT_REVIVAL_COOLDOWN = 100;
	                 ULONGLONG revivalCooldown;

};


class BillBeginState : public BillState
{

public:

	BillBeginState();
	virtual ~BillBeginState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillNormalState : public BillState
{

public:

	BillNormalState();
	virtual ~BillNormalState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillLayDownState : public BillState
{

public:

	BillLayDownState();
	virtual ~BillLayDownState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillRunShotState : public BillState
{

public:

	BillRunShotState();
	virtual ~BillRunShotState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillSwimRunState : public BillState
{

public:

	BillSwimRunState();
	virtual ~BillSwimRunState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillBeginSwimState : public BillState
{

public:

	BillBeginSwimState();
	virtual ~BillBeginSwimState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

protected:

	TIME delayTime;

};


class BillSwimNormalState : public BillState
{

public:

	BillSwimNormalState();
	virtual ~BillSwimNormalState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillNormalShotState : public BillState
{

public:

	BillNormalShotState();
	virtual ~BillNormalShotState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillStraightUpState : public BillState
{

public:

	BillStraightUpState();
	virtual ~BillStraightUpState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillSwimRunShotState : public BillState
{

public:

	BillSwimRunShotState();
	virtual ~BillSwimRunShotState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillSwimNormalShotState : public BillState
{

public:

	BillSwimNormalShotState();
	virtual ~BillSwimNormalShotState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillShotStraightUpState : public BillState
{

public:

	BillShotStraightUpState();
	virtual ~BillShotStraightUpState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillRunShotAngleUpState : public BillState
{

public:

	BillRunShotAngleUpState();
	virtual ~BillRunShotAngleUpState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillSwimShotAngleUpState : public BillState
{

public:

	BillSwimShotAngleUpState();
	virtual ~BillSwimShotAngleUpState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillRunShotAngleDownState : public BillState
{

public:

	BillRunShotAngleDownState();
	virtual ~BillRunShotAngleDownState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};


class BillSwimShotStraightUpState : public BillState
{

public:

	BillSwimShotStraightUpState();
	virtual ~BillSwimShotStraightUpState();

	virtual void Exit(Bill&) override;
	virtual void Enter(Bill&) override;
	virtual void Render(Bill&) override;

	virtual BillState* Update(Bill&) override;
	virtual BillState* HandleInput(Bill&, Input&) override;

};



















































