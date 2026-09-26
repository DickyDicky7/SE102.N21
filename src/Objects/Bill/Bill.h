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

	// Points at Scene's life counter, so a death decrements the one the HUD reads.
	int* GetLivesLeft() const;
	void SetLivesLeft(int* livesLeft);

	// Cleared on respawn; Update counts it back up to _immortalTime.
	void ResetImmortalTick();

	Bill();
	virtual ~Bill();
	virtual void GoDead();
	bool IsBill() const override;
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void  Fire                    (                               ) override;
	void  CollectItem             (ITEM_TYPE type                 );
	CollidableEntity* AsCollidable(                               ) override;
	void  StaticResolveNoCollision(                               ) override;
	void  StaticResolveOnCollision(AABBSweepResult aabbSweepResult) override;
	void DynamicResolveNoCollision(                               ) override;
	void DynamicResolveOnCollision(AABBSweepResult aabbSweepResult) override;

protected:

	int* _livesLeft;
	ULONGLONG _immortalTime;
	ULONGLONG _immortalTick;

	BillState* _state;
	BillState* _updateState;
	BillState* _handleInputState;

};


class BillState : public State<BillState, Bill>
{

public:

	BillState();
	virtual ~BillState();

	virtual void Exit(Bill& bill) override = 0;
	virtual void Enter(Bill& bill) override = 0;
	virtual void Render(Bill& bill) override = 0;

	virtual BillState* Update(Bill& bill) override = 0;
	virtual BillState* HandleInput(Bill& bill, Input& input) override = 0;

	virtual std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const;
	virtual bool IsDead() const;
	virtual bool IsInvulnerable() const;
	virtual bool IsJumping() const;
	virtual bool IsNormal() const;
	virtual bool IsFalling() const;
	virtual bool IsBeginning() const;

protected:

	float _time;

};


class BillRunState : public BillState
{

public:

	BillRunState();
	virtual ~BillRunState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

};


class BillFallState : public BillState
{

public:

	BillFallState();
	BillFallState(BillState* returnState);
	virtual ~BillFallState( );

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	bool IsFalling() const override;

protected:

	BillState* _returnState;

};


class BillJumpState : public BillState
{

public:

	BillJumpState();
	virtual ~BillJumpState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	bool IsJumping() const override;
	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

protected:

	bool _hasMovedLeft;
	bool _hasMovedRight;

};


class BillDiveState : public BillState
{

public:

	BillDiveState();
	virtual ~BillDiveState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	bool IsInvulnerable() const override;

};


class BillDeadState : public BillState
{

public:

	BillDeadState();
	virtual ~BillDeadState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	bool IsDead() const override;

protected:

	static constexpr ULONGLONG DEFAULT_REVIVAL_COOLDOWN = Constants::Bill::DEFAULT_REVIVAL_COOLDOWN_MILLISECONDS;
	                 ULONGLONG _revivalCooldown;

};


class BillBeginState : public BillState
{

public:

	BillBeginState();
	virtual ~BillBeginState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	bool IsBeginning() const override;

};


class BillNormalState : public BillState
{

public:

	BillNormalState();
	virtual ~BillNormalState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	bool IsNormal() const override;

};


class BillLayDownState : public BillState
{

public:

	BillLayDownState();
	virtual ~BillLayDownState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

};


class BillRunShotState : public BillState
{

public:

	BillRunShotState();
	virtual ~BillRunShotState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

};


class BillSwimRunState : public BillState
{

public:

	BillSwimRunState();
	virtual ~BillSwimRunState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

};


class BillBeginSwimState : public BillState
{

public:

	BillBeginSwimState();
	virtual ~BillBeginSwimState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

protected:

	TIME _delayTime;

};


class BillSwimNormalState : public BillState
{

public:

	BillSwimNormalState();
	virtual ~BillSwimNormalState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

};


class BillNormalShotState : public BillState
{

public:

	BillNormalShotState();
	virtual ~BillNormalShotState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

};


class BillStraightUpState : public BillState
{

public:

	BillStraightUpState();
	virtual ~BillStraightUpState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

};


class BillSwimRunShotState : public BillState
{

public:

	BillSwimRunShotState();
	virtual ~BillSwimRunShotState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

};


class BillSwimNormalShotState : public BillState
{

public:

	BillSwimNormalShotState();
	virtual ~BillSwimNormalShotState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

};


class BillShotStraightUpState : public BillState
{

public:

	BillShotStraightUpState();
	virtual ~BillShotStraightUpState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

};


class BillRunShotAngleUpState : public BillState
{

public:

	BillRunShotAngleUpState();
	virtual ~BillRunShotAngleUpState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

};


class BillSwimShotAngleUpState : public BillState
{

public:

	BillSwimShotAngleUpState();
	virtual ~BillSwimShotAngleUpState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

};


class BillRunShotAngleDownState : public BillState
{

public:

	BillRunShotAngleDownState();
	virtual ~BillRunShotAngleDownState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

};


class BillSwimShotStraightUpState : public BillState
{

public:

	BillSwimShotStraightUpState();
	virtual ~BillSwimShotStraightUpState();

	virtual void Exit(Bill& bill) override;
	virtual void Enter(Bill& bill) override;
	virtual void Render(Bill& bill) override;

	virtual BillState* Update(Bill& bill) override;
	virtual BillState* HandleInput(Bill& bill, Input& input) override;

	std::optional<BulletSpawnParams> GetBulletSpawnParams(const Bill& bill) const override;

};
