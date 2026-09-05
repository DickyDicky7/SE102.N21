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
		/* Input* = nullptr, */  CameraState* state = nullptr,
		float x = +Constants::Screen::WIDTH / (2.0f * _scalingRatioX), float y = +Constants::Screen::HEIGHT / (2.0f * _scalingRatioY)
	);
	~Camera();

	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void ZoomIn(float percentage = Constants::Graphics::DEFAULT_CAMERA_ZOOM_STEP);
	void ZoomOut(float percentage = Constants::Graphics::DEFAULT_CAMERA_ZOOM_STEP);
	void Capture(float x, float y);
	static float CalculateHW();
	static float CalculateHH();
	const D3DMATRIX& GetViewMatrix() const;

	void LogName() override {
		OutputDebugString(L"Camera");
	}

	bool IsStatic() const { return this->_isStatic; }
	void ToStatic();
	bool CouldSee(Entity* entity);
	float GetB() const override;
	float GetT() const override;
	float GetL() const override;
	float GetR() const override;

protected:

	// Adopts the state a handler returned: no-op when it is the current state,
	// a full ChangeState (delete outgoing, Exit/Enter) when it is a new one.
	void AdvanceState(CameraState* next);

	bool _isStatic;
	static float _scalingRatioX;
	static float _scalingRatioY;
	D3DXMATRIX _viewMatrix;
	CameraState* _state;
	D3DXVECTOR3 _eye;
	D3DXVECTOR3 _at;
	D3DXVECTOR3 _up;

};


class CameraState : public State<CameraState, Camera>
{

public:

	CameraState();
	virtual ~CameraState();

	virtual void Exit(Camera& camera) override = 0;
	virtual void Enter(Camera& camera) override = 0;
	virtual void Render(Camera& camera) override = 0;

	virtual CameraState* Update(Camera& camera) override = 0;
	virtual CameraState* Capture(float x, float y, Camera& camera) = 0;
	virtual CameraState* HandleInput(Camera& camera, Input& input) override = 0;

};


class CameraStaticState : public CameraState
{

public:

	CameraStaticState();
	virtual ~CameraStaticState();

	virtual void Exit(Camera& camera) override;
	virtual void Enter(Camera& camera) override;
	virtual void Render(Camera& camera) override;

	virtual CameraState* Update(Camera& camera) override;
	virtual CameraState* HandleInput(Camera& camera, Input& input) override;
	virtual CameraState* Capture(float x, float y, Camera& camera) override;

};


class CameraMovingUpwardState : public CameraState
{

public:

	CameraMovingUpwardState();
	virtual ~CameraMovingUpwardState();

	virtual void Exit(Camera& camera) override;
	virtual void Enter(Camera& camera) override;
	virtual void Render(Camera& camera) override;

	virtual CameraState* Update(Camera& camera) override;
	virtual CameraState* HandleInput(Camera& camera, Input& input) override;
	virtual CameraState* Capture(float x, float y, Camera& camera) override;

};


class CameraMovingForwardState : public CameraState
{

public:

	CameraMovingForwardState();
	virtual ~CameraMovingForwardState();

	virtual void Exit(Camera& camera) override;
	virtual void Enter(Camera& camera) override;
	virtual void Render(Camera& camera) override;

	virtual CameraState* Update(Camera& camera) override;
	virtual CameraState* HandleInput(Camera& camera, Input& input) override;
	virtual CameraState* Capture(float x, float y, Camera& camera) override;

};


class CameraMovingDownwardState : public CameraState
{

public:

	CameraMovingDownwardState();
	virtual ~CameraMovingDownwardState();

	virtual void Exit(Camera& camera) override;
	virtual void Enter(Camera& camera) override;
	virtual void Render(Camera& camera) override;

	virtual CameraState* Update(Camera& camera) override;
	virtual CameraState* HandleInput(Camera& camera, Input& input) override;
	virtual CameraState* Capture(float x, float y, Camera& camera) override;

};


class CameraMovingBackwardState : public CameraState
{

public:

	CameraMovingBackwardState();
	virtual ~CameraMovingBackwardState();

	virtual void Exit(Camera& camera) override;
	virtual void Enter(Camera& camera) override;
	virtual void Render(Camera& camera) override;

	virtual CameraState* Update(Camera& camera) override;
	virtual CameraState* HandleInput(Camera& camera, Input& input) override;
	virtual CameraState* Capture(float x, float y, Camera& camera) override;

};

