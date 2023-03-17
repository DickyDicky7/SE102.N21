#include "Bill.h"

Bill::Bill() : Entity()
{
	self = this;
	this->vx = 3;
	this->graphicsState = new GraphicsState();
	OutputDebugString(L"\n\Bill's constructor called\n\n");
}
Bill::~Bill()
{
	OutputDebugString(L"\n\Bill's destructor called\n\n");
}
void Bill::Update()
{
	position.x += vx;
	graphicsState->SetAnimation(0, position);
}
