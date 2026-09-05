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

// Column/row of the letter grid -> world position, in in-game pixels.
inline float AtCol(float x) { return x / (Constants::Screen::IN_GAME_WIDTH  / Letter::letterW) * Constants::Screen::IN_GAME_WIDTH ; }
inline float AtRow(float y) { return y / (Constants::Screen::IN_GAME_HEIGHT / Letter::letterH) * Constants::Screen::IN_GAME_HEIGHT; }


class Scene; class SceneState; class StartSceneState; class LoadingSceneState; class PlayingSceneState; class GameOverSceneState; class EndingSceneState; class CreditSceneState;


class Scene : public Entity, public HasTextures<Scene>, public HasSprites<Scene>, public HasAnimations<Scene>
{

public:

	Stage* GetStage() const { return this->_stage; }
	// Destroys the stage in place before adopting the new one, so a caller
	// swapping stages cannot leak the outgoing one by forgetting to.
	void ReplaceStage(Stage* stage) { Destroy(this->_stage); this->_stage = stage; }

	// Points into _livesLeftCount; Bill holds the same pointer, so a death it
	// counts down is the count the HUD reads back.
	int* GetLivesLeft() const { return this->_livesLeft; }

	bool IsStageReady() const { return this->_stageIsReady; }
	void SetStageReady(bool stageIsReady) { this->_stageIsReady = stageIsReady; }

	int  GetCurrentStage() const { return this->_currentStage; }
	void SetCurrentStage(int currentStage) { this->_currentStage = currentStage; }

	int  GetCurrentScore() const { return this->_currentScore; }
	void SetCurrentScore(int currentScore) { this->_currentScore = currentScore; }

	int  GetHighestScore() const { return this->_highestScore; }
	void SetHighestScore(int highestScore) { this->_highestScore = highestScore; }

	Scene();
	virtual ~Scene();
	void Update() override;
	void Render() override;
	void HandleInput(Input& input) override;

	void LoadSprites() override;
	void LoadTextures() override;
	void LoadAnimations() override;

protected:

	Stage* _stage;
	// Declared before _livesLeft: the constructor points that at this one, and
	// members initialise in declaration order.
	int  _livesLeftCount;
	int* _livesLeft;
	bool _stageIsReady;
	int  _currentStage;
	int  _currentScore;
	int  _highestScore;

	SceneState* _state;
	SceneState* _updateState;
	SceneState* _handleInputState;

};


class SceneState : public State<SceneState, Scene>
{

public:

	SceneState();
	virtual ~SceneState();

	virtual void Exit(Scene& scene) override = 0;
	virtual void Enter(Scene& scene) override = 0;
	virtual void Render(Scene& scene) override = 0;

	virtual SceneState* Update(Scene& scene) override = 0;
	virtual SceneState* HandleInput(Scene& scene, Input& input) override = 0;

protected:

	ULONGLONG _turn;
	ULONGLONG _time;

};


class StartSceneState : public SceneState
{

public:

	StartSceneState();
	virtual ~StartSceneState();

	virtual void Exit(Scene& scene) override;
	virtual void Enter(Scene& scene) override;
	virtual void Render(Scene& scene) override;

	virtual SceneState* Update(Scene& scene) override;
	virtual SceneState* HandleInput(Scene& scene, Input& input) override;

protected:

	bool _chosen;

};


class LoadingSceneState : public SceneState
{

public:

	LoadingSceneState();
	virtual ~LoadingSceneState();

	virtual void Exit(Scene& scene) override;
	virtual void Enter(Scene& scene) override;
	virtual void Render(Scene& scene) override;

	virtual SceneState* Update(Scene& scene) override;
	virtual SceneState* HandleInput(Scene& scene, Input& input) override;

protected:

	Text* _tLivesLeft;
	Text* _tStageNumb;
	Text* _tStageName;
	Text* _tCurrentScore;
	Text* _tHighestScore;

	Text _t1P   { "1P"  , AtRow(Constants::Scene::UI_ROW_PLAYER_ONE), AtCol(Constants::Scene::UI_COLUMN_PLAYER_ONE) };
	Text _tHi   { "HI"  , AtRow(Constants::Scene::UI_ROW_HIGH_SCORE), AtCol(Constants::Scene::UI_COLUMN_HIGH_SCORE) };
	Text _tRest { "REST", AtRow(Constants::Scene::UI_ROW_REST), AtCol(Constants::Scene::UI_COLUMN_REST) };

};


class PlayingSceneState : public SceneState
{

public:

	PlayingSceneState();
	virtual ~PlayingSceneState();

	virtual void Exit(Scene& scene) override;
	virtual void Enter(Scene& scene) override;
	virtual void Render(Scene& scene) override;

	virtual SceneState* Update(Scene& scene) override;
	virtual SceneState* HandleInput(Scene& scene, Input& input) override;

};


class GameOverSceneState : public SceneState
{

public:

	GameOverSceneState();
	virtual ~GameOverSceneState();

	virtual void Exit(Scene& scene) override;
	virtual void Enter(Scene& scene) override;
	virtual void Render(Scene& scene) override;

	virtual SceneState* Update(Scene& scene) override;
	virtual SceneState* HandleInput(Scene& scene, Input& input) override;

protected:

	enum class SELECTION { CONTINUE, END, };
			   SELECTION  _selection;
			   float      _atRow;

	Text* _tCurrentScore;
	Text* _tHighestScore;

	Text _t1P        { "1P"       , AtRow(Constants::Scene::UI_ROW_PLAYER_ONE), AtCol(Constants::Scene::UI_COLUMN_PLAYER_ONE) };
	Text _tHi        { "HI"       , AtRow(Constants::Scene::UI_ROW_HIGH_SCORE), AtCol(Constants::Scene::UI_COLUMN_HIGH_SCORE) };
	Text _tEnd       { "END"      , AtRow(Constants::Scene::UI_ROW_GAME_OVER_SELECTION_END_TEXT), AtCol(Constants::Scene::UI_COLUMN_GAME_OVER_SELECTION_END_TEXT) };
	Text _tContinue  { "CONTINUE" , AtRow(Constants::Scene::UI_ROW_GAME_OVER_SELECTION_CONTINUE_TEXT), AtCol(Constants::Scene::UI_COLUMN_GAME_OVER_SELECTION_CONTINUE_TEXT) };
	Text _tGameOver  { "GAME OVER", AtRow(Constants::Scene::UI_ROW_GAME_OVER_TITLE), AtCol(Constants::Scene::UI_COLUMN_GAME_OVER_TITLE) };

};


class EndingSceneState : public SceneState
{

public:

	EndingSceneState();
	virtual ~EndingSceneState();

	virtual void Exit(Scene& scene) override;
	virtual void Enter(Scene& scene) override;
	virtual void Render(Scene& scene) override;

	virtual SceneState* Update(Scene& scene) override;
	virtual SceneState* HandleInput(Scene& scene, Input& input) override;
protected:
	float  _delayToExplosion;
};


class CreditSceneState : public SceneState
{

public:

	CreditSceneState();
	virtual ~CreditSceneState();

	virtual void Exit(Scene& scene) override;
	virtual void Enter(Scene& scene) override;
	virtual void Render(Scene& scene) override;

	virtual SceneState* Update(Scene& scene) override;
	virtual SceneState* HandleInput(Scene& scene, Input& input) override;

protected:

	Text _tCredit1 { "CONGRATULATIONS!"              , AtRow(Constants::Scene::UI_ROW_CREDIT_LINE_1), AtCol(Constants::Scene::UI_COLUMN_CREDIT_PAGE_ONE) };
	Text _tCredit2 { "YOU'VE DESTROYED THE VILE RED" , AtRow(Constants::Scene::UI_ROW_CREDIT_LINE_2), AtCol(Constants::Scene::UI_COLUMN_CREDIT_PAGE_ONE) };
	Text _tCredit3 { "FALCON AND SAVED THE UNIVERSE.", AtRow(Constants::Scene::UI_ROW_CREDIT_LINE_3), AtCol(Constants::Scene::UI_COLUMN_CREDIT_PAGE_ONE) };
	Text _tCredit4 { "CONSIDER YOURSELF A HERO."     , AtRow(Constants::Scene::UI_ROW_CREDIT_LINE_4), AtCol(Constants::Scene::UI_COLUMN_CREDIT_PAGE_ONE) };

	Text _tCredit5{ "TEAM MEMBERS"                , AtRow(Constants::Scene::UI_ROW_CREDIT_LINE_1), AtCol(Constants::Scene::UI_COLUMN_CREDIT_PAGE_TWO) };
	Text _tCredit6{ "21522345 - PHAN VAN MINH"    , AtRow(Constants::Scene::UI_ROW_CREDIT_LINE_2), AtCol(Constants::Scene::UI_COLUMN_CREDIT_PAGE_TWO) };
	Text _tCredit7{ "21520147 - PHAM TUAN ANH"    , AtRow(Constants::Scene::UI_ROW_CREDIT_LINE_3), AtCol(Constants::Scene::UI_COLUMN_CREDIT_PAGE_TWO) };
	Text _tCredit8{ "21521307 - NGUYEN DUC PHUONG", AtRow(Constants::Scene::UI_ROW_CREDIT_LINE_4), AtCol(Constants::Scene::UI_COLUMN_CREDIT_PAGE_TWO) };

};

