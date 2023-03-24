#include "Bill.h"

#pragma region
Bill::Bill() : Entity(), HasAnimations()
{
	self = this;
	this->vx = 2;
	this->vy = 7;
	this->position.x = 50;
	this->position.y = 150;
	this->state = new BillNormalState(RIGHT);
	this->updateState = NULL;
	this->handleInputState = NULL;
	OutputDebugString(L"\n\nBill's constructor called\n\n");
}

Bill::~Bill()
{
	OutputDebugString(L"\n\nBill's destructor called\n\n");
}

void Bill::Update()
{
	updateState = state->Update(*this);
}

void Bill::Render()
{
	state->Render(*this);
	if (updateState)
	{
		state->Exit(*this);
		delete state;
		state = updateState;
		state->Enter(*this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		state->Exit(*this);
		delete state;
		state = handleInputState;
		state->Enter(*this);
		handleInputState = NULL;
	}
}

void Bill::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}
#pragma endregion Bill




















