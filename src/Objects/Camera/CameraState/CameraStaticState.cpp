#include "Camera.h"

CameraStaticState::CameraStaticState() : CameraState()
{
}

CameraStaticState::~CameraStaticState()
{
}

void CameraStaticState::Exit(Camera& camera)
{
}

void CameraStaticState::Enter(Camera& camera)
{
}

void CameraStaticState::Render(Camera& camera)
{
}

CameraState* CameraStaticState::Update(Camera& camera)
{
	return this;
}

CameraState* CameraStaticState::HandleInput(Camera& camera, Input& input)
{
	return this;
}

CameraState* CameraStaticState::Capture(FLOAT x, FLOAT y, Camera& camera)
{
	return this;
}
