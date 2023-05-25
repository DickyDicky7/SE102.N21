#pragma once
#include "Bill.h"
#include "Enemy.h"
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"

class AirCraftState;
class AirCraftNormalState;
class AirCraftBAmmoState;
class AirCraftFAmmoState;
class AirCraftLAmmoState;
class AirCraftMAmmoState;
class AirCraftRAmmoState;
class AirCraftSAmmoState;
class AirCraftInvulState;

class AirCraft : public Entity, public Enemy<Bill>
	, public HasTextures<AirCraft>, public HasSprites<AirCraft>, public HasAnimations<AirCraft>
{
public:

	AirCraft(ITEM_TYPE, AIRCRAFT_DIRECTION);
	virtual ~AirCraft();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	ITEM_TYPE getAmmoType();
	void setAmmoType(ITEM_TYPE);

	AIRCRAFT_DIRECTION getAircarftDirection() 
	{
		return _aircarftDirection;
	}
	void setAircarftDirection(AIRCRAFT_DIRECTION direction) {
		_aircarftDirection = direction;
	}
protected:

	AirCraftState* state;
	AirCraftState* updateState;
	AirCraftState* handleInputState;
	ITEM_TYPE _ammoType;
	AIRCRAFT_DIRECTION _aircarftDirection; // huong ngang hoac doc
};

// build state of aircraft
class AirCraftState : public State<AirCraftState, AirCraft>
{

public:

	AirCraftState();
	virtual ~AirCraftState();

	virtual void Exit(AirCraft&) override = 0;
	virtual void Enter(AirCraft&) override = 0;
	virtual void Render(AirCraft&) override = 0;

	virtual AirCraftState* Update(AirCraft&) override = 0;
	virtual AirCraftState* HandleInput(AirCraft&, Input&) override = 0;

protected:
	FLOAT time;
	FLOAT y0;
	FLOAT x0;
	FLOAT dt;
	FLOAT T;
	FLOAT A;
	FLOAT φ;

	// x0, y0: vị trí lúc đầu
	// time: thời gian
	// dt: delta time ( t = t + dt )
	// T: khoảng thời gian để quay hết 1 vòng (tính bằng giây)
	// A: Bán kính
	// φ: pha ban đầu của dao động (-π<φ<π)

	FLOAT v0 = 7.0f;
	FLOAT θ = 80.0f;
	// v0: vận tốc ném viên đạn
	// θ: góc ném
};
class AirCraftNormalState : public AirCraftState
{

public:

	AirCraftNormalState();
	virtual ~AirCraftNormalState();

	virtual void Exit(AirCraft&) override;
	virtual void Enter(AirCraft&) override;
	virtual void Render(AirCraft&) override;

	virtual AirCraftState* Update(AirCraft&) override;
	virtual AirCraftState* HandleInput(AirCraft&, Input&) override;

};
/// <summary>
/// Ammo state
/// There are six type of ammo: b, f, l, m, r, s
/// </summary>
class AirCraftBAmmoState : public AirCraftState
{

public:

	AirCraftBAmmoState();
	virtual ~AirCraftBAmmoState();

	virtual void Exit(AirCraft&) override;
	virtual void Enter(AirCraft&) override;
	virtual void Render(AirCraft&) override;

	virtual AirCraftState* Update(AirCraft&) override;
	virtual AirCraftState* HandleInput(AirCraft&, Input&) override;

};
class AirCraftFAmmoState : public AirCraftState
{

public:

	AirCraftFAmmoState();
	virtual ~AirCraftFAmmoState();

	virtual void Exit(AirCraft&) override;
	virtual void Enter(AirCraft&) override;
	virtual void Render(AirCraft&) override;

	virtual AirCraftState* Update(AirCraft&) override;
	virtual AirCraftState* HandleInput(AirCraft&, Input&) override;

};
class AirCraftLAmmoState : public AirCraftState
{

public:

	AirCraftLAmmoState();
	virtual ~AirCraftLAmmoState();

	virtual void Exit(AirCraft&) override;
	virtual void Enter(AirCraft&) override;
	virtual void Render(AirCraft&) override;

	virtual AirCraftState* Update(AirCraft&) override;
	virtual AirCraftState* HandleInput(AirCraft&, Input&) override;

};
class AirCraftMAmmoState : public AirCraftState
{

public:

	AirCraftMAmmoState();
	virtual ~AirCraftMAmmoState();

	virtual void Exit(AirCraft&) override;
	virtual void Enter(AirCraft&) override;
	virtual void Render(AirCraft&) override;

	virtual AirCraftState* Update(AirCraft&) override;
	virtual AirCraftState* HandleInput(AirCraft&, Input&) override;

};
class AirCraftRAmmoState : public AirCraftState
{

public:

	AirCraftRAmmoState();
	virtual ~AirCraftRAmmoState();

	virtual void Exit(AirCraft&) override;
	virtual void Enter(AirCraft&) override;
	virtual void Render(AirCraft&) override;

	virtual AirCraftState* Update(AirCraft&) override;
	virtual AirCraftState* HandleInput(AirCraft&, Input&) override;

};
class AirCraftSAmmoState : public AirCraftState
{

public:

	AirCraftSAmmoState();
	virtual ~AirCraftSAmmoState();

	virtual void Exit(AirCraft&) override;
	virtual void Enter(AirCraft&) override;
	virtual void Render(AirCraft&) override;

	virtual AirCraftState* Update(AirCraft&) override;
	virtual AirCraftState* HandleInput(AirCraft&, Input&) override;

};
/// <summary>
/// Invul state
/// </summary>
class AirCraftInvulState : public AirCraftState
{

public:

	AirCraftInvulState();
	virtual ~AirCraftInvulState();

	virtual void Exit(AirCraft&) override;
	virtual void Enter(AirCraft&) override;
	virtual void Render(AirCraft&) override;

	virtual AirCraftState* Update(AirCraft&) override;
	virtual AirCraftState* HandleInput(AirCraft&, Input&) override;

};
