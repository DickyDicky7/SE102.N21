#pragma once


#include "State.h"
#include "Entity.h"
#include "Common.h"


class Camera;
class CameraState;
class CameraStaticState;
class CameraMovingUpwardState;
class CameraMovingForwardState;
class CameraMovingDownwardState;
class CameraMovingBackwardState;


class Camera : public Entity
{

public:

	Camera
	(
		/* Input* = NULL, */  CameraState* = NULL,
		FLOAT = +SCREEN_WIDTH / (2.0f * scalingRatioX), FLOAT = +SCREEN_HEIGHT / (2.0f * scalingRatioY)
	);
	~Camera();

	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void ZoomIn(FLOAT = 0.1f);
	void ZoomOut(FLOAT = 0.1f);
	void Capture(FLOAT, FLOAT);
	static FLOAT CalculateHW();
	static FLOAT CalculateHH();
	const D3DMATRIX& GetViewMatrix() const;

	void LogName() override {
		OutputDebugString(L"Camera");
	}

protected:

	static FLOAT scalingRatioX;
	static FLOAT scalingRatioY;
	D3DXMATRIX viewMatrix;
	CameraState* state;
	D3DXVECTOR3 eye;
	D3DXVECTOR3 at;
	D3DXVECTOR3 up;

};


class CameraState : public State<CameraState, Camera>
{

public:

	CameraState();
	virtual ~CameraState();

	virtual void Exit(Camera&) override = 0;
	virtual void Enter(Camera&) override = 0;
	virtual void Render(Camera&) override = 0;

	virtual CameraState* Update(Camera&) override = 0;
	virtual CameraState* Capture(FLOAT, FLOAT, Camera&) = 0;
	virtual CameraState* HandleInput(Camera&, Input&) override = 0;

};


class CameraStaticState : public CameraState
{

public:

	CameraStaticState();
	virtual ~CameraStaticState();

	virtual void Exit(Camera&) override;
	virtual void Enter(Camera&) override;
	virtual void Render(Camera&) override;

	virtual CameraState* Update(Camera&) override;
	virtual CameraState* HandleInput(Camera&, Input&) override;
	virtual CameraState* Capture(FLOAT, FLOAT, Camera&) override;

};


class CameraMovingUpwardState : public CameraState
{

public:

	CameraMovingUpwardState();
	virtual ~CameraMovingUpwardState();

	virtual void Exit(Camera&) override;
	virtual void Enter(Camera&) override;
	virtual void Render(Camera&) override;

	virtual CameraState* Update(Camera&) override;
	virtual CameraState* HandleInput(Camera&, Input&) override;
	virtual CameraState* Capture(FLOAT, FLOAT, Camera&) override;

};


class CameraMovingForwardState : public CameraState
{

public:

	CameraMovingForwardState();
	virtual ~CameraMovingForwardState();

	virtual void Exit(Camera&) override;
	virtual void Enter(Camera&) override;
	virtual void Render(Camera&) override;

	virtual CameraState* Update(Camera&) override;
	virtual CameraState* HandleInput(Camera&, Input&) override;
	virtual CameraState* Capture(FLOAT, FLOAT, Camera&) override;

};


class CameraMovingDownwardState : public CameraState
{

public:

	CameraMovingDownwardState();
	virtual ~CameraMovingDownwardState();

	virtual void Exit(Camera&) override;
	virtual void Enter(Camera&) override;
	virtual void Render(Camera&) override;

	virtual CameraState* Update(Camera&) override;
	virtual CameraState* HandleInput(Camera&, Input&) override;
	virtual CameraState* Capture(FLOAT, FLOAT, Camera&) override;

};


class CameraMovingBackwardState : public CameraState
{

public:

	CameraMovingBackwardState();
	virtual ~CameraMovingBackwardState();

	virtual void Exit(Camera&) override;
	virtual void Enter(Camera&) override;
	virtual void Render(Camera&) override;

	virtual CameraState* Update(Camera&) override;
	virtual CameraState* HandleInput(Camera&, Input&) override;
	virtual CameraState* Capture(FLOAT, FLOAT, Camera&) override;

};

