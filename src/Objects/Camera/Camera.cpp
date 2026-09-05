#include "Camera.h"

float Camera::_scalingRatioX = Constants::Screen::SCALING_X;
float Camera::_scalingRatioY = Constants::Screen::SCALING_Y;

Camera::Camera(CameraState* state, float x, float y) : Entity()
{
	this->_state = state;
	this->_isStatic = false;
	this->_position.x = x;
	this->_position.y = y;
}

Camera::~Camera()
{
	Destroy(this->_state);
}

// Advances camera state, deleting the outgoing state and running Exit/Enter handlers.
void Camera::AdvanceState(CameraState* next)
{
	if (next == this->_state) return;

	ChangeState(this->_state, next, this);
}

void Camera::Update()
{
	if (this->_state)
		this->AdvanceState(this->_state->Update(*this));
}

void Camera::Render()
{
	if (this->_state)
		this->_state->Render(*this);
}

void Camera::HandleInput(Input& input)
{
	if (this->_state)
		this->AdvanceState(this->_state->HandleInput(*this, input));

	if (input.IsWheelUp()) this->ZoomIn();
	if (input.IsWheelDown()) this->ZoomOut();
}

void Camera::ZoomIn(float percentage)
{
	Camera::_scalingRatioX += percentage;
	Camera::_scalingRatioY += percentage;
}

void Camera::ZoomOut(float percentage)
{
	if (Camera::_scalingRatioX - percentage > 0) Camera::_scalingRatioX -= percentage;
	if (Camera::_scalingRatioY - percentage > 0) Camera::_scalingRatioY -= percentage;
}

void Camera::Capture(float x, float y)
{
	if (this->_state)
		this->AdvanceState(this->_state->Capture(x, y, *this));

	// World to View transform (Y-up world space)
	this->_eye = D3DXVECTOR3(+this->_position.x, +this->_position.y, -1.0f);
	this->_at  = D3DXVECTOR3(+this->_position.x, +this->_position.y, +0.0f);
	this->_up  = D3DXVECTOR3(+0.0f, +1.0f, +0.0f);

	D3DXMatrixLookAtLH(&this->_viewMatrix, &this->_eye, &this->_at, &this->_up);

	// Zoom is applied after centring to scale about camera centre
	D3DXMATRIX scalingMatrix;
	D3DXMatrixScaling(&scalingMatrix, Camera::_scalingRatioX, Camera::_scalingRatioY, 1.0f);

	this->_viewMatrix *= scalingMatrix;
}

float Camera::CalculateHW()
{
	return +Constants::Screen::WIDTH  / (2.0f * Camera::_scalingRatioX);
}

float Camera::CalculateHH()
{
	return +Constants::Screen::HEIGHT / (2.0f * Camera::_scalingRatioY);
}

const D3DMATRIX& Camera::GetViewMatrix() const
{
	return this->_viewMatrix;
}

void Camera::ToStatic()
{
	this->_isStatic = true;
	ChangeState(this->_state, new CameraStaticState(), this);
}

bool Camera::CouldSee(Entity* entity)
{
	return !(entity->GetB() >= this->GetT()
		||   entity->GetT() <= this->GetB()
		||   entity->GetL() >= this->GetR()
		||   entity->GetR() <= this->GetL())
		;
}

// The visible rectangle, used by CouldSee to cull.  These go through
// CalculateHW/HH so they read the LIVE _scalingRatioX/Y rather than the
// SCALING_RATIO_* macros the ratios were merely initialised from: the mouse
// wheel changes the ratios at runtime, and culling against the un-zoomed
// rectangle makes entities vanish inside the frame (or linger outside it).
float Camera::GetB() const
{
	return this->_position.y - this->CalculateHH();
}

float Camera::GetT() const
{
	return this->_position.y + this->CalculateHH();
}

float Camera::GetL() const
{
	return this->_position.x - this->CalculateHW();
}

float Camera::GetR() const
{
	return this->_position.x + this->CalculateHW();
}
