#include "Camera.h"

CameraMovingUpwardState::CameraMovingUpwardState() : CameraState()
{
}

CameraMovingUpwardState::~CameraMovingUpwardState()
{
}

void CameraMovingUpwardState::Exit(Camera& camera)
{
}

void CameraMovingUpwardState::Enter(Camera& camera)
{
}

void CameraMovingUpwardState::Render(Camera& camera)
{
}

CameraState* CameraMovingUpwardState::Update(Camera& camera)
{
	return this;
}

CameraState* CameraMovingUpwardState::HandleInput(Camera& camera, Input& input)
{
	return this;
}

CameraState* CameraMovingUpwardState::Capture(FLOAT x, FLOAT y, Camera& camera)
{
	if (y >= camera.GetY())
	{
		camera.SetY(y);
	}
	if (y <= camera.GetY() - Camera::CalculateHH())
	{
		camera.SetY(camera.GetY() - 2.0f * Camera::CalculateHH());
		return new CameraMovingDownwardState();
	}
	return this;
}
