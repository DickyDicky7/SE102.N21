#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"

class BossStage3Hand;

// 1 Joint la 1 khop xuong
class BossStage3Joint : public Entity, public Enemy<Bill>
	, public HasTextures<BossStage3Joint>, public HasSprites<BossStage3Joint>, public HasAnimations<BossStage3Joint>
{
public:

	enum class MoveAroundDirection
	{
		Positive, // clockwise
		Negative, // counter-clockwise
		Unknown   // default
	};

	BossStage3Joint(BOSS_STAGE_3_HAND_ANIMATION_ID type, D3DXVECTOR3 positionInit, DIRECTION direction, BossStage3Hand* parent);
	virtual ~BossStage3Joint();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

	// Moves with velocity vx, vy for frames count
	virtual void MoveBy(float x, float y, int frames, int delayFrames = 0);

	// Moves in a circle around (x, y) with radius and speed (degrees / frame)
	virtual void MoveAround(float x, float y, float radius, int frames, float speed, MoveAroundDirection direction, float accelerator = 0, int delayFrames = 0);

	virtual void MoveFollow(BossStage3Joint* joint, bool isMoveFollowWithAccelerator = true);

	virtual void SetMoveAroundSpeed(float speed);

	virtual void SetMoveAroundRadius(float radius);

	virtual void ReverseMoveAroundDirection();

	virtual void SetMoveAroundDelay(int frame);

	virtual float GetAngleSpeed();

	virtual void StopMoveFollow();

	virtual int GetMoveAroundCurrFrame();

	virtual D3DXVECTOR2 GetCurrentMoveDirection();

	virtual void StopMoveAround();

	virtual void AlignDistance(BossStage3Joint* joint2); // Keeps joint1 distance to joint2 fixed

	bool IsEnemy() const override { return true; }
	void SetTarget(const Bill* target) override { this->Enemy<Bill>::SetTarget(target); }
	bool TakeBulletHit() override;


	bool IsMoveAround() const { return this->_isMoveAround; }
	bool IsMoveTo() const { return this->_isMoveTo; }
	bool IsMoveBy() const { return this->_isMoveBy; }
	bool IsMoveFollow() const { return this->_isMoveFollow; }

	float GetDistanceMoved() const { return this->_distanceMoved; }
	float GetAngleMoved() const { return this->_angleMoved; }
	float GetMoveAroundSpeed() const { return this->_moveAroundSpeed; }
	MoveAroundDirection GetMoveAroundDirection() const { return this->_moveAroundDirection; }
	BossStage3Hand* GetParent() const { return this->_parent; }

protected:
	bool _isMoveAround;
	bool _isMoveTo;
	bool _isMoveBy;
	bool _isMoveFollow;

	float _distanceMoved;
	float _angleMoved;
	float _moveAroundSpeed; // (Degree / frame)

	BossStage3Hand* _parent;
	MoveAroundDirection _moveAroundDirection;

	BOSS_STAGE_3_HAND_ANIMATION_ID _type; // type of hand: hand or arm

	void UpdateMoveBy(float oldx, float oldy);
	void UpdateMoveFollow();
	void UpdateMoveTo(float oldx, float oldy);
	void UpdateMoveAround();

	float _moveAroundX;
	float _moveAroundY;
	float _moveAroundRadius;
	float _moveAroundAccelerator;
	float _moveToX;
	float _moveToY;

	int _moveAroundFrame;
	int _moveAroundDelay;
	int _moveAroundCurrFrame;
	int _moveToFrame;
	int _moveToCurrFrame;
	int _moveToDelay;
	int _moveByFrame;
	int _moveByCurrFrame;
	int _moveByDelay;

	D3DXVECTOR2 _currMoveDirection;

	BossStage3Joint* _jointFollow;

	bool _isMoveAroundAccelerator;
	bool _allowMoveReverse;
	bool _isJointFollowStopped;
	bool _isMoveFollowFirstTime;
	bool _isMoveFollowWithAccelerator;
};
