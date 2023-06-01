#pragma once
#include "State.h"
#include "Common.h"
#include "Entity.h"
#include "HasAnimations.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "Bill.h"
#include "Enemy.h"

// 1 Joint la 1 khop xuong
class BossStage3Joint : public Entity, public Enemy<Bill>
	, public HasTextures<BossStage3Joint>, public HasSprites<BossStage3Joint>, public HasAnimations<BossStage3Joint>
{
public:

    enum MoveAroundDirection
    {
        Positive, //chieu duong - chieu kim dong ho
        Negative, //chieu am
        Unknow //default
    };

	BossStage3Joint(BOSS_STAGE_3_HAND_ANIMATION_ID, D3DXVECTOR3, DIRECTION);
	virtual ~BossStage3Joint();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

    //di chuyen theo van toc vx, vy trong vong frames Frame
    virtual void moveBy(float x, float y, int frames, int delayFrame = 0);

    //di chuyen vong tron quanh toa do x,y ban kinh radius voi van toc speed (Degree / frame) - chuyen dong deu
    virtual void moveAround(float x, float y, float radius, int frames, float speed, MoveAroundDirection direction, float accelemetor = 0, int delayFrame = 0);

    virtual void moveFollow(BossStage3Joint* joint, bool isMoveFollowWithAccelemetor = true);

    virtual void setMoveAroundSpeed(float speed);

    virtual void setMoveAroundRadius(float radius);

    virtual void reverseMoveAroundDirection();

    virtual void setMoveAroundDelay(int frame);

    virtual float getAngleSpeed();

    virtual void stopMoveFollow();

    virtual int getMoveAroundCurrFrame();

    virtual D3DXVECTOR2 getCurrentMoveDirection();

    virtual void stopMoveAround();

    virtual void alignDistance(BossStage3Joint* joint2); // lam cho joint1 luon cach joint2 1 khoang = tong ban kinh 2 thang luon co dinh


    bool    isMoveAround,
            isMoveTo,
            isMoveBy,
            isMoveByVelocity,
            isMoveFollow;

    float distanceMoved;
    float angleMoved;
    float moveAroundSpeed; // (Degree / frame)

    MoveAroundDirection moveAroundDirection;

protected:
	BOSS_STAGE_3_HAND_ANIMATION_ID type; // type of hand: hand or arm

    void updateMoveBy(float oldx, float oldy);
    void updateMoveFollow();
    void updateMoveTo(float oldx, float oldy);
    void updateMoveAround();

    float   moveAroundX,
            moveAroundY,
            moveAroundRadius,
            moveAroundDelay,
            moveAroundFrame,
            moveAroundAccelemetor,
            moveToX,
            moveToY,
            moveToFrame,
            moveToCurrFrame,
            moveByCurrFrame,
            moveByFrame,
            moveByDelay;

    int moveToDelay, moveAroundCurrFrame;

    D3DXVECTOR2 currMoveDirection;

    BossStage3Joint* jointFollow;

	bool isMoveAroundAcclemetor, allowMoveReverse, isJointFollowStoped, isMoveFollowFirstTime, isMoveFollowWithAccelemetor;
};