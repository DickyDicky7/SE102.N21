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
	, public HasTextures<BossStage3Hand>, public HasSprites<BossStage3Hand>, public HasAnimations<BossStage3Hand>, public HasWeapons
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

	void  Fire() override;
	void  Fire(FLOAT, FLOAT, FLOAT, FLOAT);

	BOOL GetIsFire() { return isFire; }
	void SetIsFire(BOOL check) { isFire = check; }

	// 5 cuc xuong
	BossStage3Joint* joints[5];
protected:
	BossStage3HandState* state;
	BossStage3HandState* updateState;
	BossStage3HandState* handleInputState;

	BOOL isInitPositionJoints; // check init has init postion of joints
	BOOL isFire;
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


class BossStage3HandWaveState : public BossStage3HandState
{

public:

	BossStage3HandWaveState(BossStage3Hand&);
	virtual ~BossStage3HandWaveState();

	virtual void Exit(BossStage3Hand&) override;
	virtual void Enter(BossStage3Hand&) override;
	virtual void Render(BossStage3Hand&) override;

	virtual BossStage3HandState* Update(BossStage3Hand&) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand&, Input&) override;

protected:
	float limitFrame; //degree
	float speed; //degree / frame
	int currFrame, changeStateFrame;
	int moveAroundDelay;

	std::vector<D3DXVECTOR2> listJoint1Angle;

	void moveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius, float speed, float frame,
		BossStage3Joint::MoveAroundDirection dir, float delay = 0);

	//lay goc giua 2 diem
	float getAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);
};

class BossStage3HandSpinningState : public BossStage3HandState
{

public:

	BossStage3HandSpinningState(BossStage3Hand&);
	virtual ~BossStage3HandSpinningState();

	virtual void Exit(BossStage3Hand&) override;
	virtual void Enter(BossStage3Hand&) override;
	virtual void Render(BossStage3Hand&) override;

	virtual BossStage3HandState* Update(BossStage3Hand&) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand&, Input&) override;

protected:
	void moveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius, float speed, float frame,
		BossStage3Joint::MoveAroundDirection dir, float delay = 0);
	float getAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);

	int FRAME_ATTACK;
	float speedAngle;
	float numRound; //so vong xoay de doi state
	int totalFrame;
	int delayChangeState;
	int frameAttack;
	BossStage3Joint** joints;
};

class BossStage3HandAttackState : public BossStage3HandState
{

public:

	BossStage3HandAttackState(BossStage3Hand&);
	virtual ~BossStage3HandAttackState();

	virtual void Exit(BossStage3Hand&) override;
	virtual void Enter(BossStage3Hand&) override;
	virtual void Render(BossStage3Hand&) override;

	virtual BossStage3HandState* Update(BossStage3Hand&) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand&, Input&) override;

	void moveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius, float speed, float frame,
		BossStage3Joint::MoveAroundDirection dir, float delay = 0);

	float getAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);

protected:
	int delayChangeState;
	int frameAttack;
	BossStage3Joint** joints;
};

class BossStage3HandDirectPlayerState : public BossStage3HandState
{

public:

	BossStage3HandDirectPlayerState(BossStage3Hand&);
	virtual ~BossStage3HandDirectPlayerState();

	virtual void Exit(BossStage3Hand&) override;
	virtual void Enter(BossStage3Hand&) override;
	virtual void Render(BossStage3Hand&) override;

	virtual BossStage3HandState* Update(BossStage3Hand&) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand&, Input&) override;

protected:
	//goc cua vec1 so voi vec2
	float getAngle2Vector(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);
	float getAngleBetweenPlayerAndjoint(BossStage3Joint* joint, BossStage3Hand& bossStage3hand);
	D3DXVECTOR3 getNearestPlayer(BossStage3Hand& bossStage3hand);
	void moveAroundDirect(BossStage3Hand& bossStage3hand, BossStage3Joint* joint0, BossStage3Joint* joint, float speed, float radius = 16);
	float getAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);
	BossStage3Joint** joints;
	float speed;
	bool isFirstTime;
	int frameDelayChangeState;
	int timeAttack;
};