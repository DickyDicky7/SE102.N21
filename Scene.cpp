#include "Scene.h"

Scene::Scene() : Entity(), HasTextures(), HasSprites(), HasAnimations(), state(NULL), updateState(NULL), handleInputState(NULL)
{
	ChangeState(state, new StartSceneState, this);
}

Scene::~Scene()
{
	Destroy(state);
	Destroy(updateState);
	Destroy(handleInputState);
}

void Scene::Update()
{
	updateState = state->Update(*this);
}

void Scene::Render()
{
	state->Render(*this);
	this->w = this->currentFrameW;
	this->h = this->currentFrameH;

	if (updateState)
	{
		ChangeState(state, updateState, this);
		updateState = NULL;
	}
	if (handleInputState)
	{
		ChangeState(state, handleInputState, this);
		handleInputState = NULL;
	}
}

void Scene::HandleInput(Input& input)
{
	handleInputState = state->HandleInput(*this, input);
}

void Scene::LoadSprites()
{
	if (HasSprites<Scene>::hasBeenLoaded.value) return;
	HasSprites<Scene>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertSprite(SCENE_SPRITE_ID::START_SCENE, 0, 0, 256, 224, DIRECTION::LEFT, SCENE_TEXTURE_ID::START_SCENE);

	OutputDebugString(L"Scene Sprites Loaded Successfully\n");
}

void Scene::LoadTextures()
{
	if (HasTextures<Scene>::hasBeenLoaded.value) return;
	HasTextures<Scene>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertTexure(SCENE_TEXTURE_ID::START_SCENE, L"Resources\\Textures\\StartScene.bmp");

	OutputDebugString(L"Scene Textures Loaded Successfully\n");
}

void Scene::LoadAnimations()
{
	if (HasAnimations<Scene>::hasBeenLoaded.value) return;
	HasAnimations<Scene>::hasBeenLoaded.value = true;

	GraphicsHelper::InsertAnimation(SCENE_ANIMATION_ID::START_SCENE, 300, { { SCENE_SPRITE_ID::START_SCENE, 0 } });

	OutputDebugString(L"Scene Animations Loaded Successfully\n");
}
