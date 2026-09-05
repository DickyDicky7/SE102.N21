#include "Scene.h"

// Initialiser order follows the declaration order in Scene.h; _livesLeft points
// at _livesLeftCount, so that one has to be initialised first.
Scene::Scene() : Entity(), HasTextures(), HasSprites(), HasAnimations()
	, _stage(nullptr)
	, _livesLeftCount(Constants::Scene::INITIAL_LIVES_COUNT)
	, _livesLeft(&_livesLeftCount)
	, _stageIsReady(false)
	, _currentStage(0)
	, _currentScore(0)
	, _highestScore(Constants::Scene::DEFAULT_HIGH_SCORE)
	, _state(nullptr), _updateState(nullptr), _handleInputState(nullptr)
{
	ChangeState(this->_state, new StartSceneState(), this);
	this->LoadTextures(); this->LoadSprites(); this->LoadAnimations();
	Letter representativeLetter(""); representativeLetter.LoadTextures(); representativeLetter.LoadSprites(); representativeLetter.LoadAnimations();
}

Scene::~Scene()
{
	Destroy(this->_state);
	Destroy(this->_updateState);
	Destroy(this->_handleInputState);
	Destroy(this->_stage);
}

void Scene::Update()
{
	DeferState(this->_updateState, this->_state->Update(*this));

	ApplyDeferredState(this->_state, this->_updateState, this);
	ApplyDeferredState(this->_state, this->_handleInputState, this);
}

void Scene::Render()
{
	this->_state->Render(*this);
	this->_w = this->GetCurrentFrameW();
	this->_h = this->GetCurrentFrameH();
}

void Scene::HandleInput(Input& input)
{
	DeferState(this->_handleInputState, this->_state->HandleInput(*this, input));
}

void Scene::LoadSprites()
{
	if (HasSprites<Scene>::_hasBeenLoaded) return;
	HasSprites<Scene>::_hasBeenLoaded = true;

	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::LIFE         , 000, 015, 030, 020, DIRECTION::LEFT, SCENE_TEXTURE_ID::LIFE         );
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::BLACK        , 000, 000, static_cast<int>(Constants::Scene::BLACK_OVERLAY_WIDTH), static_cast<int>(Constants::Scene::BLACK_OVERLAY_HEIGHT), DIRECTION::LEFT, SCENE_TEXTURE_ID::BLACK        );
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::BLANK        , 000, 000, 100, 013, DIRECTION::LEFT, SCENE_TEXTURE_ID::BLANK        );
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::START_SCENE  , 000, 000, static_cast<int>(Constants::Scene::START_SCENE_WIDTH), static_cast<int>(Constants::Scene::START_SCENE_HEIGHT), DIRECTION::LEFT, SCENE_TEXTURE_ID::START_SCENE  );
	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::YELLOW_FALCON, 000, 000,  18, 014, DIRECTION::LEFT, SCENE_TEXTURE_ID::YELLOW_FALCON);

	for (int i = 0; i < Constants::Scene::ENDING_TOTAL_FRAME_COUNT; ++i)
	{
		SCENE_SPRITE_ID spriteId = static_cast<SCENE_SPRITE_ID>(static_cast<int>(SCENE_SPRITE_ID::ENDING_FRAME_00) + i);
		SCENE_TEXTURE_ID textureId = static_cast<SCENE_TEXTURE_ID>(static_cast<int>(SCENE_TEXTURE_ID::ENDING_FRAME_00) + i);
		GraphicsHelper::InsertSprite(spriteId, 0, 0, static_cast<int>(Constants::Scene::BLACK_OVERLAY_WIDTH), static_cast<int>(Constants::Scene::BLACK_OVERLAY_HEIGHT), DIRECTION::LEFT, textureId);
	}

	OutputDebugString(L"Scene Sprites Loaded Successfully\n");
}

void Scene::LoadTextures()
{
	if (HasTextures<Scene>::_hasBeenLoaded) return;
	HasTextures<Scene>::_hasBeenLoaded = true;

	GraphicsHelper::InsertTexture(SCENE_TEXTURE_ID::LIFE         , L"Resources\\Textures\\Life.bmp"        );
	GraphicsHelper::InsertTexture(SCENE_TEXTURE_ID::BLACK        , L"Resources\\Textures\\Black.bmp"       );
	GraphicsHelper::InsertTexture(SCENE_TEXTURE_ID::BLANK        , L"Resources\\Textures\\Blank.bmp"       );
	GraphicsHelper::InsertTexture(SCENE_TEXTURE_ID::START_SCENE  , L"Resources\\Textures\\StartScene.bmp"  );
	GraphicsHelper::InsertTexture(SCENE_TEXTURE_ID::YELLOW_FALCON, L"Resources\\Textures\\YellowFalcon.bmp");

	for (int i = 0; i < Constants::Scene::ENDING_TOTAL_FRAME_COUNT; ++i)
	{
		SCENE_TEXTURE_ID textureId = static_cast<SCENE_TEXTURE_ID>(static_cast<int>(SCENE_TEXTURE_ID::ENDING_FRAME_00) + i);
		wchar_t pathBuf[Constants::Scene::FILEPATH_BUFFER_MAX_LENGTH];
		swprintf_s(pathBuf, L"Resources\\Textures\\ending_frame\\frame_%02d_delay-0.1s.bmp", i);
		GraphicsHelper::InsertTexture(textureId, pathBuf);
	}

	OutputDebugString(L"Scene Textures Loaded Successfully\n");
}

void Scene::LoadAnimations()
{
	if (HasAnimations<Scene>::_hasBeenLoaded) return;
	HasAnimations<Scene>::_hasBeenLoaded = true;

	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::LIFE         , Constants::Graphics::DEFAULT_ANIMATION_DELAY_MILLISECONDS, { { SCENE_SPRITE_ID::LIFE         , 0 } });
	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::BLACK        , Constants::Graphics::DEFAULT_ANIMATION_DELAY_MILLISECONDS, { { SCENE_SPRITE_ID::BLACK        , 0 } });
	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::BLANK        , Constants::Graphics::DEFAULT_ANIMATION_DELAY_MILLISECONDS, { { SCENE_SPRITE_ID::BLANK        , 0 } });
	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::START_SCENE  , Constants::Graphics::DEFAULT_ANIMATION_DELAY_MILLISECONDS, { { SCENE_SPRITE_ID::START_SCENE  , 0 } });
	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::YELLOW_FALCON, Constants::Graphics::DEFAULT_ANIMATION_DELAY_MILLISECONDS, { { SCENE_SPRITE_ID::YELLOW_FALCON, 0 } });

	std::vector<std::tuple<SPRITE_ID, TIME>> endingFrames;
	endingFrames.reserve(Constants::Scene::ENDING_TOTAL_FRAME_COUNT);
	for (int i = 0; i < Constants::Scene::ENDING_TOTAL_FRAME_COUNT; ++i)
	{
		SCENE_SPRITE_ID spriteId = static_cast<SCENE_SPRITE_ID>(static_cast<int>(SCENE_SPRITE_ID::ENDING_FRAME_00) + i);
		endingFrames.emplace_back(spriteId, 0);
	}
	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::ENDING_FRAME, Constants::Scene::ENDING_FRAME_DELAY_MILLISECONDS, endingFrames);

	OutputDebugString(L"Scene Animations Loaded Successfully\n");
}
