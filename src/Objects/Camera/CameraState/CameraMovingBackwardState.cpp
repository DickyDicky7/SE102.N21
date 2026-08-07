#include "Camera.h"

CameraMovingBackwardState::CameraMovingBackwardState() : CameraState()
{
}

CameraMovingBackwardState::~CameraMovingBackwardState()
{
}

void CameraMovingBackwardState::Exit(Camera& camera)
{
}

void CameraMovingBackwardState::Enter(Camera& camera)
{
}

void CameraMovingBackwardState::Render(Camera& camera)
{
}

CameraState* CameraMovingBackwardState::Update(Camera& camera)
{
	return this;
}

CameraState* CameraMovingBackwardState::HandleInput(Camera& camera, Input& input)
{
	return this;
}

CameraState* CameraMovingBackwardState::Capture(FLOAT x, FLOAT y, Camera& camera)
{
	if (x <= camera.GetX())
	{
		camera.SetX(x);
	}
	if (x >= camera.GetX() + Camera::CalculateHW())
	{
		camera.SetX(camera.GetX() + 2.0f * Camera::CalculateHW());
		return new CameraMovingForwardState();
	}
	return this;
}
