#include "Camera.h"

CameraMovingDownwardState::CameraMovingDownwardState() : CameraState()
{
}

CameraMovingDownwardState::~CameraMovingDownwardState()
{
}

void CameraMovingDownwardState::Exit(Camera& camera)
{
}

void CameraMovingDownwardState::Enter(Camera& camera)
{
}

void CameraMovingDownwardState::Render(Camera& camera)
{
}

CameraState* CameraMovingDownwardState::Update(Camera& camera)
{
	return this;
}

CameraState* CameraMovingDownwardState::HandleInput(Camera& camera, Input& input)
{
	return this;
}

CameraState* CameraMovingDownwardState::Capture(FLOAT x, FLOAT y, Camera& camera)
{
	if (y <= camera.GetY())
	{
		camera.SetY(y);
	}
	if (y >= camera.GetY() + Camera::CalculateHH())
	{
		camera.SetY(camera.GetY() + 2.0f * Camera::CalculateHH());
		return new CameraMovingUpwardState();
	}
	return this;
}
