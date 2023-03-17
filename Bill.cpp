#include "Bill.h"

Bill::Bill() : Entity()
{
	self = this;
	this->vx = 2;
	this->vy = 3;
	this->position.x = 100;
	this->position.y = 300;
	//this->graphicsState = new GraphicsState();
	OutputDebugString(L"\n\Bill's constructor called\n\n");
}
Bill::~Bill()
{
	OutputDebugString(L"\n\Bill's destructor called\n\n");
}
void Bill::Update()
{
	position.y += vy;
	if (position.y < 100 || position.y > 300) vy = -vy;
	//graphicsState->SetAnimation(BILL_RUN, position);
	SetAnimation(BILL_JUMP);
}
