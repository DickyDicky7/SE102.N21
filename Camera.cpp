#include "Camera.h"

Camera::Camera(CameraState* state, FLOAT x, FLOAT y) : Entity()
{
	self = this;
	this->state = state;
	this->position.x = x;
	this->position.y = y;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (state)
		state = state->Update(*this);
}

void Camera::Render()
{
	if (state)
		state->Render(*this);
}

void Camera::HandleInput(Input& input)
{
	if (state)
		state = state->HandleInput(*this, input);
}

void Camera::Capture(FLOAT x, FLOAT y)
{
	if (state)
		state = state->Capture(x, y, *this);

	eye = D3DXVECTOR3
	(
		+position.x, -position.y, -1.0f
	);
	at = D3DXVECTOR3
	(
		+position.x, -position.y, +0.0f
	);
	up = D3DXVECTOR3
	(
		+0.0f, +1.0f, +0.0f
	);

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);

	D3DXMATRIX scalingMatrix;
	D3DXMatrixScaling(&scalingMatrix, SCALING_RATIO_X, SCALING_RATIO_Y, 1.0f);

	viewMatrix *= scalingMatrix;
}

const D3DMATRIX& Camera::GetViewMatrix() const
{
	return viewMatrix;
}
