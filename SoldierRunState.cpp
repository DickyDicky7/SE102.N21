#include "Soldier.h"

SoldierRunState::SoldierRunState() : SoldierState()
{
}
SoldierRunState::~SoldierRunState()
{
}

void SoldierRunState::Exit(Soldier&)
{
	// exit here
}
void SoldierRunState::Enter(Soldier& soldier) {
	if (soldier.GetMovingDirection() == DIRECTION::LEFT)
	{
		soldier.SetVX(+1.00f);
		soldier.SetAX(+0.01f);
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(-1.00f);
		soldier.SetAX(-0.01f);
	}
}

void SoldierRunState::Render(Soldier& soldier) {
	soldier.SetAnimation(SOLDIER_ANIMATION_ID::RUN, soldier.GetPosition(), soldier.GetMovingDirection(), soldier.GetAngle());
}

SoldierState* SoldierRunState::Update(Soldier& soldier) {
	float solVX = soldier.GetVX();
	float solAX = soldier.GetAX();
	float solX = soldier.GetX();
	const float WIDTH_SOLDIE = 520;

	if (soldier.GetMovingDirection() == DIRECTION::LEFT)
	{
		soldier.SetVX(-abs(solVX));
		soldier.SetAX(-abs(solAX));
	}
	if (soldier.GetMovingDirection() == DIRECTION::RIGHT)
	{
		soldier.SetVX(+abs(solVX));
		soldier.SetAX(+abs(solAX));
	}

	// soldier moves left and right of the screen
	if (solX <= 0) {
		// if you hit the left wall
		soldier.SetMovingDirection(DIRECTION::RIGHT);
	}
	// i find that the soldier will go over left screen about WIDTH_SOLDIE so i minus it
	else if (solX >= SCREEN_WIDTH - WIDTH_SOLDIE) {
		// if you hit the left wall
		soldier.SetMovingDirection(DIRECTION::LEFT);
	}

	// set X of soldier
	soldier.SetX
	(
		// x = x0 + v0*t -- uniform accelerated motion
		solX + solVX * time

	);

	// Restrict Ox velocity and time. Make this part "v0*t + a*(t^2)/2" become a constant => uniform motion
	// If not, entity will move too fast. The code here is temporary
	if (time < +2.00f)
	{
		time += 0.05f;
	}

	return NULL;
}
SoldierState* SoldierRunState::HandleInput(Soldier& soldier, Input& input) {

	if (input.IsKey(DIK_SPACE))
	{
		return new SoldierJumpState();
	}
	if (input.IsKey(DIK_DOWN))
	{
		return new SoldierLayDownState();
	}
	return NULL;
}
