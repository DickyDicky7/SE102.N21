#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "BossStage3Joint.h"
#include "Bill.h"
#include "Enemy.h"

class BossStage3Hand;
class BossStage3HandState;
class BossStage3HandStartState; // dai ra tu tu
class BossStage3HandWaveState; // vay vay
class BossStage3HandSpinningState; // quay
class BossStage3HandAttackState; // ban dan ve nguoi choi
class BossStage3HandDirectPlayerState; // xoay ve phia nguoi choi

class BossStage3Hand : public Entity, public Enemy<Bill>
	, public HasTextures<BossStage3Hand>, public HasSprites<BossStage3Hand>, public HasAnimations<BossStage3Hand>
{
public:
	BossStage3Hand();
	virtual ~BossStage3Hand();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void initPositionJoints();

	// 5 cuc xuong
	BossStage3Joint* joints[5];
protected:
	BossStage3HandState* state;
	BossStage3HandState* updateState;
	BossStage3HandState* handleInputState;

	BOOL isInitPositionJoints; // check init has init postion of joints
};



class BossStage3HandState : public State<BossStage3HandState, BossStage3Hand>
{

public:

	BossStage3HandState();
	virtual ~BossStage3HandState();

	virtual void Exit(BossStage3Hand&) override = 0;
	virtual void Enter(BossStage3Hand&) override = 0;
	virtual void Render(BossStage3Hand&) override = 0;

	virtual BossStage3HandState* Update(BossStage3Hand&) override = 0;
	virtual BossStage3HandState* HandleInput(BossStage3Hand&, Input&) override = 0;
};


class BossStage3HandStartState : public BossStage3HandState
{

public:

	BossStage3HandStartState(BossStage3Hand&);
	virtual ~BossStage3HandStartState();

	virtual void Exit(BossStage3Hand&) override;
	virtual void Enter(BossStage3Hand&) override;
	virtual void Render(BossStage3Hand&) override;

	virtual BossStage3HandState* Update(BossStage3Hand&) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand&, Input&) override;

protected:
	D3DXVECTOR2 direction;
	FLOAT speedFrame, distance, delayFrame;
};
