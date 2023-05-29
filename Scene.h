#pragma once


#include "State.h"
#include "Letter.h"
#include "Motion.h"
#include "Common.h"
#include "Entity.h"
//#include "Stage1.h"
//#include "Stage2.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"


static constexpr FLOAT IN_GAME_SCREEN_W = SCREEN_WIDTH  / SCALING_RATIO_X;
static constexpr FLOAT IN_GAME_SCREEN_H = SCREEN_HEIGHT / SCALING_RATIO_Y;


inline FLOAT AtCol(FLOAT x) { return x / (IN_GAME_SCREEN_W / Letter::letterW) * IN_GAME_SCREEN_W; }
inline FLOAT AtRow(FLOAT y) { return y / (IN_GAME_SCREEN_H / Letter::letterH) * IN_GAME_SCREEN_H; }


class Scene; class SceneState; class StartSceneState; class LoadingSceneState; class Stage1SceneState; class Stage2SceneState;


class Scene : public Entity, public HasTextures<Scene>, public HasSprites<Scene>, public HasAnimations<Scene>
{

public:

	Scene();
	virtual ~Scene();
	void Update() override;
	void Render() override;
	void HandleInput(Input&) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

protected:

	SceneState* state;
	SceneState* updateState;
	SceneState* handleInputState;

};


class SceneState : public State<SceneState, Scene>
{

public:

	SceneState();
	virtual ~SceneState();

	virtual void Exit(Scene&) override = 0;
	virtual void Enter(Scene&) override = 0;
	virtual void Render(Scene&) override = 0;

	virtual SceneState* Update(Scene&) override = 0;
	virtual SceneState* HandleInput(Scene&, Input&) override = 0;

protected:

	FLOAT time;

};


class StartSceneState : public SceneState
{

public:

	StartSceneState();
	virtual ~StartSceneState();

	virtual void Exit(Scene&) override;
	virtual void Enter(Scene&) override;
	virtual void Render(Scene&) override;

	virtual SceneState* Update(Scene&) override;
	virtual SceneState* HandleInput(Scene&, Input&) override;

};


class LoadingSceneState : public SceneState
{

public:

	LoadingSceneState();
	virtual ~LoadingSceneState();

	virtual void Exit(Scene&) override;
	virtual void Enter(Scene&) override;
	virtual void Render(Scene&) override;

	virtual SceneState* Update(Scene&) override;
	virtual SceneState* HandleInput(Scene&, Input&) override;

protected:

	Text T_1P  { "1P"  , AtRow(25.0f), AtCol(5.00f) };
	Text T_HI  { "HI"  , AtRow(24.0f), AtCol(5.00f) };
	Text T_REST{ "REST", AtRow(20.0f), AtCol(10.0f) };

};


class Stage1SceneState : public SceneState
{

public:

	Stage1SceneState();
	virtual ~Stage1SceneState();

	virtual void Exit(Scene&) override;
	virtual void Enter(Scene&) override;
	virtual void Render(Scene&) override;

	virtual SceneState* Update(Scene&) override;
	virtual SceneState* HandleInput(Scene&, Input&) override;

};


class Stage2SceneState : public SceneState
{

public:

	Stage2SceneState();
	virtual ~Stage2SceneState();

	virtual void Exit(Scene&) override;
	virtual void Enter(Scene&) override;
	virtual void Render(Scene&) override;

	virtual SceneState* Update(Scene&) override;
	virtual SceneState* HandleInput(Scene&, Input&) override;

};

