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
	if (x >= camera.GetX()) camera.SetX(x);
	return this;
}
