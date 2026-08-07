#include "Camera.h"

FLOAT Camera::scalingRatioX = SCALING_RATIO_X;
FLOAT Camera::scalingRatioY = SCALING_RATIO_Y;

Camera::Camera(CameraState* state, FLOAT x, FLOAT y) : Entity()
{
	this->state = state;
	this->isStatic   = 0;
	this->position.x = x;
	this->position.y = y;
}

Camera::~Camera()
{
	Destroy(state);
}

// Advances camera state, deleting the outgoing state and running Exit/Enter handlers.
void Camera::AdvanceState(CameraState* next)
{
	if (next == state) return;

	ChangeState(state, next, this);
}

void Camera::Update()
{
	if (state)
		AdvanceState(state->Update(*this));
}

void Camera::Render()
{
	if (state)
		state->Render(*this);
}

void Camera::HandleInput(Input& input)
{
	if (state)
		AdvanceState(state->HandleInput(*this, input));

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
		AdvanceState(state->Capture(x, y, *this));

	// World to View transform (Y-up world space)
	eye = D3DXVECTOR3(+position.x, +position.y, -1.0f);
	at  = D3DXVECTOR3(+position.x, +position.y, +0.0f);
	up  = D3DXVECTOR3(+0.0f, +1.0f, +0.0f);

	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);

	// Zoom is applied after centring to scale about camera centre
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

// The visible rectangle, used by CouldSee to cull.  These go through
// CalculateHW/HH so they read the LIVE scalingRatioX/Y rather than the
// SCALING_RATIO_* macros the ratios were merely initialised from: the mouse
// wheel changes the ratios at runtime, and culling against the un-zoomed
// rectangle makes entities vanish inside the frame (or linger outside it).
FLOAT Camera::GetB() const
{
	return position.y - CalculateHH();
}

FLOAT Camera::GetT() const
{
	return position.y + CalculateHH();
}

FLOAT Camera::GetL() const
{
	return position.x - CalculateHW();
}

FLOAT Camera::GetR() const
{
	return position.x + CalculateHW();
}
