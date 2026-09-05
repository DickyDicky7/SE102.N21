#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"

class Bridge;
class BridgePart;
class BridgePartState;
class BridgePartExplosionState; // this one is special so i think need to split it

class Bridge : public Entity, public Enemy<Bill>
	, public HasTextures<Bridge>, public HasSprites<Bridge>, public HasAnimations<Bridge>
{
public:
	Bridge();
	Bridge(D3DXVECTOR3 position, float w);
	virtual ~Bridge();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void InitBridgePart();

	bool IsBridge() const override { return true; }
	bool IsWalkableSurface() const override { return true; }
	bool IsEnemy() const override { return true; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
protected:
	BridgePart* _bridgePart[Constants::Enemies::Bridge::PART_COUNT];
	bool _isInitBridge;
	int _lastDestroyPart = -1;
};

class BridgePart : public Entity, public Enemy<Bill>, public HasAnimations<BridgePart>
{
public:
	BridgePart();
	BridgePart(ANIMATION_ID animationId);
	virtual ~BridgePart();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;
	void LoadAnimations() override;
	void SetIsDestroy(bool isDestroy);
	bool GetIsDestroy() const;

	bool IsBridge() const override { return true; }
	bool IsWalkableSurface() const override { return true; }
	bool IsEnemy() const override { return true; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
protected:
	bool _isDestroy;
	BridgePartState* _state;
	BridgePartState* _updateState;
};

class BridgePartState : public State<BridgePartState, BridgePart>
{
public:
	BridgePartState();
	BridgePartState(ANIMATION_ID animationId);
	virtual ~BridgePartState();

	virtual void Exit(BridgePart& bridgePart);
	virtual void Enter(BridgePart& bridgePart);
	virtual void Render(BridgePart& bridgePart);

	virtual BridgePartState* Update(BridgePart& bridgePart);
	virtual BridgePartState* HandleInput(BridgePart& bridgePart, Input& input) override;

	virtual bool HasZeroDimensions() const { return false; }
protected:
	float _time;
	ANIMATION_ID _animationId;
};


class BridgePartExplosionState : public BridgePartState
{
public:
	BridgePartExplosionState();
	~BridgePartExplosionState();

	virtual void Exit(BridgePart& bridgePart);
	virtual void Enter(BridgePart& bridgePart);
	virtual void Render(BridgePart& bridgePart);

	virtual BridgePartState* Update(BridgePart& bridgePart);

	bool HasZeroDimensions() const override { return true; }
};
