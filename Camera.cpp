#include "Camera.h"

FLOAT Camera::scalingRatioX = SCALING_RATIO_X;
FLOAT Camera::scalingRatioY = SCALING_RATIO_Y;

Camera::Camera(/* Input* input, */ CameraState* state, FLOAT x, FLOAT y) : Entity()
{
	//if (input)
	//{
	//	input->SetMouseEventListener
	//	(
	//		MOUSE_EVENT_HANDLER
	//		(
	//			[](LPDIRECTINPUTDEVICE8 mouse, DIMOUSESTATE& buttons) -> void
	//			{
	//				OutputDebugString(L"aaa\n");
	//			}
	//		)
	//	);
	//}

	this->state = state;
	this->isStatic   = 0;
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

	if (input.IsWheelUp()) ZoomIn();
	if (input.IsWheelDown()) ZoomOut();
}

void Camera::ZoomIn(FLOAT percentage)
{
	scalingRatioX += percentage;
	scalingRatioY += percentage;
}

void Camera::ZoomOut(FLOAT percentage)
{
	if (scalingRatioX - percentage > 0) scalingRatioX -= percentage;
	if (scalingRatioY - percentage > 0) scalingRatioY -= percentage;
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
	D3DXMatrixScaling(&scalingMatrix, scalingRatioX, scalingRatioY, 1.0f);

	viewMatrix *= scalingMatrix;
}

FLOAT Camera::CalculateHW()
{
	return +SCREEN_WIDTH  / (2.0f * scalingRatioX);
}

FLOAT Camera::CalculateHH()
{
	return +SCREEN_HEIGHT / (2.0f * scalingRatioY);
}

const D3DMATRIX& Camera::GetViewMatrix() const
{
	return viewMatrix;
}

void Camera::ToStatic()
{
	isStatic = 1;
	ChangeState(state, new CameraStaticState(), this);
}

BOOL Camera::CouldSee(Entity* entity)
{
	return !(entity->GetB() >= this->GetT()
		||   entity->GetT() <= this->GetB()
		||   entity->GetL() >= this->GetR()
		||   entity->GetR() <= this->GetL())
		;
}

FLOAT Camera::GetB() const
{
	return position.y - SCREEN_HEIGHT / SCALING_RATIO_Y * 0.5f;
}

FLOAT Camera::GetT() const
{
	return position.y + SCREEN_HEIGHT / SCALING_RATIO_Y * 0.5f;
}

FLOAT Camera::GetL() const
{
	return position.x - SCREEN_WIDTH / SCALING_RATIO_X * 0.5f;
}

FLOAT Camera::GetR() const
{
	return position.x + SCREEN_WIDTH / SCALING_RATIO_X * 0.5f;
}
