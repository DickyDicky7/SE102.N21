#pragma once


#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"


class Bullet;
class BulletState;
class BulletRState;


class Bullet : public Entity<Bullet>, public HasTextures<Bullet>, public HasSprites<Bullet>, public HasAnimations<Bullet>
{

public:

	Bullet();
	virtual ~Bullet();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

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