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
#include "Sound.h"

class BossStage3Hand;
class BossStage3HandState;
class BossStage3HandStartState;        // Extends outward gradually
class BossStage3HandWaveState;         // Waves back and forth
class BossStage3HandSpinningState;     // Spins
class BossStage3HandAttackState;       // Fires bullets at the player
class BossStage3HandDirectPlayerState; // Rotates to aim at the player

class BossStage3Hand : public Entity, public Enemy<Bill>
	, public HasTextures<BossStage3Hand>, public HasSprites<BossStage3Hand>, public HasAnimations<BossStage3Hand>, public HasWeapons
{
public:
	BossStage3Hand();
	virtual ~BossStage3Hand();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	void InitPositionJoints();

	void  Fire() override;
	void  Fire(float x, float y, float vx, float vy);

	bool GetIsFire() { return this->_isFire; }
	void SetIsFire(bool check) { this->_isFire = check; }

	// Fixed-length bone chain: index 0 is the shoulder anchor and the last index
	// is the hand itself, so the roles below are tied to this exact count.
	static_assert(Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT == 5,
		"Joint indices 0/1/4 are hardcoded across the BossStage3Hand states.");

	BossStage3Joint* GetJoint(size_t index) const { return this->_joints[index]; }
	// The chain itself, for a state that wants to hold on to it across frames.
	// Pointer-to-const-pointer: the joints stay mutable, the wiring does not.
	BossStage3Joint* const* GetJoints() const { return this->_joints; }

	bool IsEnemy() const override { return true; }
	ENEMY_TYPE GetEnemyType() const override { return this->_enemyType; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
	void ForEachCollisionEntity(std::function<void(Entity*)> callback) override;
	void ProcessSpecialDeathEffects(std::vector<Entity*>& effectEntities) override;
	bool TakeBulletHit() override { return this->Enemy<Bill>::TakeEnemyBulletHit(this); }
protected:
	BossStage3Joint* _joints[Constants::Enemies::BossStage3::Hand::TOTAL_JOINTS_COUNT];

	BossStage3HandState* _state;
	BossStage3HandState* _updateState;
	BossStage3HandState* _handleInputState;

	bool _isInitPositionJoints; // Whether the joints' start positions have been set
	bool _isFire;
};



class BossStage3HandState : public State<BossStage3HandState, BossStage3Hand>
{

public:

	BossStage3HandState();
	virtual ~BossStage3HandState();

	virtual void Exit(BossStage3Hand& bossStage3Hand) override = 0;
	virtual void Enter(BossStage3Hand& bossStage3Hand) override = 0;
	virtual void Render(BossStage3Hand& bossStage3Hand) override;

	virtual BossStage3HandState* Update(BossStage3Hand& bossStage3Hand) override = 0;
	virtual BossStage3HandState* HandleInput(BossStage3Hand& bossStage3Hand, Input& input) override = 0;

protected:
	float GetAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);
};


class BossStage3HandStartState : public BossStage3HandState
{

public:

	BossStage3HandStartState(BossStage3Hand& bossStage3Hand);
	virtual ~BossStage3HandStartState();

	virtual void Exit(BossStage3Hand& bossStage3Hand) override;
	virtual void Enter(BossStage3Hand& bossStage3Hand) override;

	virtual BossStage3HandState* Update(BossStage3Hand& bossStage3Hand) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand& bossStage3Hand, Input& input) override;

protected:
	D3DXVECTOR2 _direction;
	float _speedFrame;
	float _distance;
	float _delayFrame;
};


class BossStage3HandWaveState : public BossStage3HandState
{

public:

	BossStage3HandWaveState(BossStage3Hand& bossStage3Hand);
	virtual ~BossStage3HandWaveState();

	virtual void Exit(BossStage3Hand& bossStage3Hand) override;
	virtual void Enter(BossStage3Hand& bossStage3Hand) override;

	virtual BossStage3HandState* Update(BossStage3Hand& bossStage3Hand) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand& bossStage3Hand, Input& input) override;

protected:
	float _limitFrame; //degree
	float _speed; //degree / frame
	int _currFrame;
	int _changeStateFrame;
	int _moveAroundDelay;

	void MoveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius, float speed, float frame,
		BossStage3Joint::MoveAroundDirection dir, float accelerator = 0);

	// Angle between two points
	float GetAngle(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);
};

class BossStage3HandSpinningState : public BossStage3HandState
{

public:

	BossStage3HandSpinningState(BossStage3Hand& bossStage3Hand);
	virtual ~BossStage3HandSpinningState();

	virtual void Exit(BossStage3Hand& bossStage3Hand) override;
	virtual void Enter(BossStage3Hand& bossStage3Hand) override;

	virtual BossStage3HandState* Update(BossStage3Hand& bossStage3Hand) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand& bossStage3Hand, Input& input) override;

protected:
	void MoveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius, float speed, float frame,
		BossStage3Joint::MoveAroundDirection dir, float accelerator = 0);

	float _speedAngle;
	float _numRound; // Spin revolutions to complete before changing state
	int _totalFrame;
	int _delayChangeState;
	int _frameAttack;
	BossStage3Joint* const* _joints;
};

class BossStage3HandAttackState : public BossStage3HandState
{

public:

	BossStage3HandAttackState(BossStage3Hand& bossStage3Hand);
	virtual ~BossStage3HandAttackState();

	virtual void Exit(BossStage3Hand& bossStage3Hand) override;
	virtual void Enter(BossStage3Hand& bossStage3Hand) override;

	virtual BossStage3HandState* Update(BossStage3Hand& bossStage3Hand) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand& bossStage3Hand, Input& input) override;

	void MoveAround(BossStage3Joint* joint, BossStage3Joint* joint0, float deltaRadius, float speed, float frame,
		BossStage3Joint::MoveAroundDirection dir, float accelerator = 0);

protected:
	int _delayChangeState;
	int _frameAttack;
	BossStage3Joint* const* _joints;
};

class BossStage3HandDirectPlayerState : public BossStage3HandState
{

public:

	BossStage3HandDirectPlayerState(BossStage3Hand& bossStage3Hand);
	virtual ~BossStage3HandDirectPlayerState();

	virtual void Exit(BossStage3Hand& bossStage3Hand) override;
	virtual void Enter(BossStage3Hand& bossStage3Hand) override;

	virtual BossStage3HandState* Update(BossStage3Hand& bossStage3Hand) override;
	virtual BossStage3HandState* HandleInput(BossStage3Hand& bossStage3Hand, Input& input) override;

protected:
	// Angle of vec1 relative to vec2
	float GetAngle2Vector(D3DXVECTOR3 vec1, D3DXVECTOR3 vec2);
	float GetAngleBetweenPlayerAndJoint(BossStage3Joint* joint, BossStage3Hand& bossStage3Hand);
	D3DXVECTOR3 GetNearestPlayer(BossStage3Hand& bossStage3Hand);
	void MoveAroundDirect(BossStage3Hand& bossStage3Hand, BossStage3Joint* joint0, BossStage3Joint* joint, float speed, float radius = Constants::Enemies::BossStage3::Hand::JOINT_MOVE_AROUND_RADIUS);
	BossStage3Joint* const* _joints;
	float _speed;
	bool _isFirstTime;
	int _frameDelayChangeState;
	int _timeAttack;
};
