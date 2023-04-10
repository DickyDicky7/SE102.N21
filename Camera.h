#pragma once

#include "Common.h"

class Camera
{

public:

	Camera();
	virtual ~Camera();
	virtual void CaptureX(FLOAT);
	virtual void CaptureY(FLOAT);

	virtual Camera* SetLastX(FLOAT);
	virtual Camera* SetLastY(FLOAT);
	virtual Camera* SetSpeedX(FLOAT);
	virtual Camera* SetSpeedY(FLOAT);

	virtual FLOAT GetLastX() const;
	virtual FLOAT GetLastY() const;
	virtual FLOAT GetSpeedX() const;
	virtual FLOAT GetSpeedY() const;
	virtual const D3DMATRIX& GetViewMatrix() const;

protected:

	D3DXMATRIX viewMatrix;
	D3DXVECTOR3 eye;
	D3DXVECTOR3 at;
	D3DXVECTOR3 up;
	FLOAT speedX;
	FLOAT speedY;
	FLOAT lastX;
	FLOAT lastY;

};

