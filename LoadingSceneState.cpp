#include "Scene.h"

LoadingSceneState::LoadingSceneState() : SceneState()
{
	this->time = 200.0f;
}

LoadingSceneState::~LoadingSceneState()
{
}

void LoadingSceneState::Exit(Scene& scene)
{
}

void LoadingSceneState::Enter(Scene& scene)
{
	std::jthread newThread
	(
		[](Scene& scene)
		{
			scene.safeToUseStage = false;
			if (!scene.stage)
			{
				scene.stage = new Stage1();
				scene.stage->Load<TerrainStage1, CameraMovingForwardState>();
			}
			else
			if (dynamic_cast<Stage1*>(scene.stage))
			{
				Destroy(scene.stage);
				scene.stage = new Stage2();
				scene.stage->Load<TerrainStage2, CameraMovingUpwardState >();
			}
			else
			if (dynamic_cast<Stage2*>(scene.stage))
			{
				Destroy(scene.stage);
				scene.stage = new Stage1();
				scene.stage->Load<TerrainStage1, CameraMovingForwardState>();
			}
			std::this_thread::sleep_for(std::chrono::seconds(5));
			scene.safeToUseStage = true;
		}
		,   std::ref(scene)
	);
	newThread.detach();
}

void LoadingSceneState::Render(Scene& scene)
{
	T_1P.Render(); T_HI.Render(); T_REST.Render();
}

SceneState* LoadingSceneState::Update(Scene& scene)
{
	if (scene.safeToUseStage) return new PlayingSceneState();
	return NULL;
}

SceneState* LoadingSceneState::HandleInput(Scene& scene, Input& input)
{
	return NULL;
}
