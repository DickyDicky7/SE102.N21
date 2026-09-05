#include "Camera.h"

CameraMovingForwardState::CameraMovingForwardState() : CameraState()
{
}

CameraMovingForwardState::~CameraMovingForwardState()
{
}

void CameraMovingForwardState::Exit(Camera& camera)
{
}

void CameraMovingForwardState::Enter(Camera& camera)
{
}

void CameraMovingForwardState::Render(Camera& camera)
{
}

CameraState* CameraMovingForwardState::Update(Camera& camera)
{
	return this;
}

CameraState* CameraMovingForwardState::HandleInput(Camera& camera, Input& input)
{
	return this;
}

CameraState* CameraMovingForwardState::Capture(float x, float y, Camera& camera)
{
	if (x >= camera.GetX())
	{
		camera.SetX(x);
	}
	if (x <= camera.GetX() - Camera::CalculateHW())
	{
		camera.SetX(camera.GetX() - Constants::Graphics::CAMERA_TRANSITION_STEP_FACTOR * Camera::CalculateHW());
		return new CameraMovingBackwardState();
	}
	return this;
}
