#include "Bill.h"

Bill::Bill() : Entity(), HasAnimations()
{
	self = this;

	this->vx = 1;
	this->vy = 1;
	this->ax = 0.1;
	this->ay = 0.1;
	this->position.x = SCREEN_WIDTH / 4;
	this->position.y = SCREEN_HEIGHT / 2 - 50;

	this->updateState = NULL;
	this->handleInputState = NULL;
	this->state = new BillNormalState();
}

Bill::~Bill()
{
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