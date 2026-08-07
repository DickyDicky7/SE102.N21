#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"

#define BRIDGE_PART 4

class Bridge;
class BridgePart;
class BridgePartState;
class BridgePartExplosionState; // this one is special so i think need to split it

class Bridge : public Entity, public Enemy<Bill>
	, public HasTextures<Bridge>, public HasSprites<Bridge>, public HasAnimations<Bridge>
{
public:
	Bridge();
	Bridge(D3DXVECTOR3, FLOAT);
	virtual ~Bridge();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void InitBridgePart();

protected:
	BridgePart* bridgePart[BRIDGE_PART];
	BOOL isInitBridge;
	int lastDestroyPart = -1;
};

class BridgePart : public Entity, public Enemy<Bill>, public HasAnimations<BridgePart>
{
public:
	BridgePart();
	BridgePart(ANIMATION_ID);
	virtual ~BridgePart();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;
	void LoadAnimations() override;
	void SetIsDestroy(BOOLEAN);
	BOOLEAN GetIsDestroy();

protected:
	BOOLEAN isDestroy;
	BridgePartState* state;
	BridgePartState* updateState;
};

class BridgePartState : public State<BridgePartState, BridgePart>
{
public:
	BridgePartState();
	BridgePartState(ANIMATION_ID);
	virtual ~BridgePartState();

	virtual void Exit(BridgePart&);
	virtual void Enter(BridgePart&);
	virtual void Render(BridgePart&);

	virtual BridgePartState* Update(BridgePart&);
	virtual BridgePartState* HandleInput(BridgePart&, Input&) override;
protected:
	FLOAT time;
	ANIMATION_ID animationId;
};


class BridgePartExplosionState : public BridgePartState
{
public:
	BridgePartExplosionState();
	~BridgePartExplosionState();

	virtual void Exit(BridgePart&);
	virtual void Enter(BridgePart&);
	virtual void Render(BridgePart&);

	virtual BridgePartState* Update(BridgePart&);
};