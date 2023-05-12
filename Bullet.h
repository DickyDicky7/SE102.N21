#pragma once


#include "State.h"
#include "Motion.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"
#include "CollidableEntity.h"


class Bullet;
class BulletState;
class BulletRState;
class BulletMState;
class BulletSState;
class BulletLState;
class BulletFState;


class Bullet : public Entity, public HasTextures<Bullet>, public HasSprites<Bullet>, public HasAnimations<Bullet>, public CollidableEntity
{

public:

	Bullet(            );
	Bullet(BulletState*);
	virtual ~Bullet();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void SetState(BulletState*);
	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void  StaticResolveNoCollision(               ) override;
	void  StaticResolveOnCollision(AABBSweepResult) override;
	void DynamicResolveNoCollision(               ) override;
	void DynamicResolveOnCollision(AABBSweepResult) override;

protected:

	BulletState* state;
	BulletState* updateState;
	BulletState* handleInputState;

};


class BulletState : public State<BulletState, Bullet>
{

public:

	BulletState();
	virtual ~BulletState();

	virtual void Exit(Bullet&) override = 0;
	virtual void Enter(Bullet&) override = 0;
	virtual void Render(Bullet&) override = 0;

	virtual BulletState* Update(Bullet&) override = 0;
	virtual BulletState* HandleInput(Bullet&, Input&) override = 0;

protected:

	FLOAT time;

};


class BulletRState : public BulletState
{

public:

	BulletRState();
	virtual ~BulletRState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletMState : public BulletState
{

public:

	BulletMState();
	virtual ~BulletMState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletSState : public BulletState
{

public:

	BulletSState();
	virtual ~BulletSState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	static const std::vector<FLOAT> spreadDegrees;
	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletLState : public BulletState
{

public:

	BulletLState();
	virtual ~BulletLState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

};


class BulletFState : public BulletState
{

public:

	BulletFState
	(
		FLOAT = 0.0f,
		FLOAT = 0.0f,
		FLOAT = 0.0f
	);
	virtual ~BulletFState();

	virtual void Exit(Bullet&) override;
	virtual void Enter(Bullet&) override;
	virtual void Render(Bullet&) override;

	virtual BulletState* Update(Bullet&) override;
	virtual BulletState* HandleInput(Bullet&, Input&) override;

protected:

	FLOAT r;
	FLOAT ω;
	FLOAT dω;
	FLOAT xO;
	FLOAT yO;

};
