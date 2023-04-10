#include "Camera.h"

Camera::Camera() : eye(0.0f, 0.0f, -1.0f), at(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f), speedX(1.0f), speedY(1.0f), lastX(0.0f), lastY(0.0f)
{
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
}

Camera::~Camera()
{
}

void Camera::CaptureX(FLOAT x)
{
	if (x > lastX)
	{
		at.x += speedX;
		eye.x += speedX;
	}
	if (x < lastX)
	{
		at.x -= speedX;
		eye.x -= speedX;
	}

	lastX = x;
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
}

void Camera::CaptureY(FLOAT y)
{
	if (y > lastY)
	{
		at.y -= speedY;
		eye.y -= speedY;
	}
	if (y < lastY)
	{
		at.y += speedY;
		eye.y += speedY;
	}

	lastY = y;
	D3DXMatrixLookAtLH(&viewMatrix, &eye, &at, &up);
}

Camera* Camera::SetLastX(FLOAT lastX) { this->lastX = lastX; return this; }

Camera* Camera::SetLastY(FLOAT lastY) { this->lastY = lastY; return this; }

Camera* Camera::SetSpeedX(FLOAT speedX) { this->speedX = speedX; return this; }

Camera* Camera::SetSpeedY(FLOAT speedY) { this->speedY = speedY; return this; }

FLOAT Camera::GetLastX() const { return lastX; }

FLOAT Camera::GetLastY() const { return lastY; }

FLOAT Camera::GetSpeedX() const { return speedX; }

FLOAT Camera::GetSpeedY() const { return speedY; }

const D3DMATRIX& Camera::GetViewMatrix() const { return viewMatrix; }
