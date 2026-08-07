#pragma once


#include "Bill.h"
#include "State.h"
#include "Letter.h"
#include "Motion.h"
#include "Common.h"
#include "Entity.h"
#include "Stage1.h"
#include "Stage2.h"
#include "HasSprites.h"
#include "HasTextures.h"
#include "HasAnimations.h"
#include "TerrainStage1.h"
#include "TerrainStage2.h"
#include "Sound.h"

static constexpr FLOAT IN_GAME_SCREEN_W = SCREEN_WIDTH  / SCALING_RATIO_X;
static constexpr FLOAT IN_GAME_SCREEN_H = SCREEN_HEIGHT / SCALING_RATIO_Y;


inline FLOAT AtCol(FLOAT x) { return x / (IN_GAME_SCREEN_W / Letter::letterW) * IN_GAME_SCREEN_W; }
inline FLOAT AtRow(FLOAT y) { return y / (IN_GAME_SCREEN_H / Letter::letterH) * IN_GAME_SCREEN_H; }


class Scene; class SceneState; class StartSceneState; class LoadingSceneState; class PlayingSceneState; class GameOverSceneState; class EndingSceneState; class CreditSceneState;


class Scene : public Entity, public HasTextures<Scene>, public HasSprites<Scene>, public HasAnimations<Scene>
{

public:

	Stage* stage; INT* livesLeft; BOOL stageIsReady;
	INT currentStage;
	INT currentScore;
	INT highestScore;

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

	ULONGLONG turn;
	ULONGLONG time;

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

protected:

	BOOL chosen;

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

	Text* T_LIVES_LEFT;
	Text* T_STAGE_NUMB;
	Text* T_STAGE_NAME;
	Text* T_CURRENT_SCORE;
	Text* T_HIGHEST_SCORE;

	Text T_1P   { "1P"  , AtRow(25.0f), AtCol(5.00f) };
	Text T_HI   { "HI"  , AtRow(19.0f), AtCol(10.0f) };
	Text T_REST { "REST", AtRow(23.0f), AtCol(5.00f) };

};


class PlayingSceneState : public SceneState
{

public:

	PlayingSceneState();
	virtual ~PlayingSceneState();

	virtual void Exit(Scene&) override;
	virtual void Enter(Scene&) override;
	virtual void Render(Scene&) override;

	virtual SceneState* Update(Scene&) override;
	virtual SceneState* HandleInput(Scene&, Input&) override;

};


class GameOverSceneState : public SceneState
{

public:

	GameOverSceneState();
	virtual ~GameOverSceneState();

	virtual void Exit(Scene&) override;
	virtual void Enter(Scene&) override;
	virtual void Render(Scene&) override;

	virtual SceneState* Update(Scene&) override;
	virtual SceneState* HandleInput(Scene&, Input&) override;

protected:

	enum class SELECTION { CONTINUE, END, };
			   SELECTION  selection;
			   FLOAT      atRow;
	
	Text* T_CURRENT_SCORE;
	Text* T_HIGHEST_SCORE;

	Text T_1P        { "1P"       , AtRow(25.0f), AtCol(5.00f) };
	Text T_HI        { "HI"       , AtRow(19.0f), AtCol(10.0f) };
	Text T_END       { "END"      , AtRow(09.0f), AtCol(13.0f) };
	Text T_CONTINUE  { "CONTINUE" , AtRow(11.0f), AtCol(13.0f) };
	Text T_GAME_OVER { "GAME OVER", AtRow(14.0f), AtCol(12.0f) };

};


class EndingSceneState : public SceneState
{

public:

	EndingSceneState();
	virtual ~EndingSceneState();

	virtual void Exit(Scene&) override;
	virtual void Enter(Scene&) override;
	virtual void Render(Scene&) override;

	virtual SceneState* Update(Scene&) override;
	virtual SceneState* HandleInput(Scene&, Input&) override;
protected: 
	float  delayToExplosion;
};


class CreditSceneState : public SceneState
{

public:

	CreditSceneState();
	virtual ~CreditSceneState();

	virtual void Exit(Scene&) override;
	virtual void Enter(Scene&) override;
	virtual void Render(Scene&) override;

	virtual SceneState* Update(Scene&) override;
	virtual SceneState* HandleInput(Scene&, Input&) override;

protected:

	Text T_CREDIT_1 { "CONGRATULATIONS!"              , AtRow(16.0f), AtCol(1.0f) };
	Text T_CREDIT_2 { "YOU'VE DESTROYED THE VILE RED" , AtRow(14.0f), AtCol(1.0f) };
	Text T_CREDIT_3 { "FALCON AND SAVED THE UNIVERSE.", AtRow(12.0f), AtCol(1.0f) };
	Text T_CREDIT_4 { "CONSIDER YOURSELF A HERO."     , AtRow(10.0f), AtCol(1.0f) };

	Text T_CREDIT_5{ "TEAM MEMBERS"                , AtRow(16.0f), AtCol(2.0f) };
	Text T_CREDIT_6{ "21522345 - PHAN VAN MINH"    , AtRow(14.0f), AtCol(2.0f) };
	Text T_CREDIT_7{ "21520147 - PHAM TUAN ANH"    , AtRow(12.0f), AtCol(2.0f) };
	Text T_CREDIT_8{ "21521307 - NGUYEN DUC PHUONG", AtRow(10.0f), AtCol(2.0f) };

};

