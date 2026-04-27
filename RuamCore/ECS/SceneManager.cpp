#include "SceneManager.h"
//#include "Serial.h"
#include "EditorCamera.h"
#include "Editor.h"
#include "GameCamera.h"
#include "ModelRenderer.h"
#include "Renderer.h"
#include "Engine.h"

#include "SaveSystem.h"

#include "GlobalLight.h"
#include "SaveSystem.h"
#include "Scene.h"
#include "Serial.h"

#include <memory>


namespace RuamEngine
{
	std::vector<std::string> SceneManager::s_scenes;
	SceneManager::SceneUPtr SceneManager::s_activeScene = nullptr;
	bool SceneManager::s_pendingSceneChange = false;
	bool SceneManager::s_pendingLoadCameraPos = false;
	std::string SceneManager::s_pendingSceneName;
	const std::vector<std::string>& SceneManager::Scenes()
	{
		return s_scenes;
	}

	void SceneManager::ChangeActiveScene(const std::string &sceneName)
	{
		Editor::s_selectedEntity = nullptr;
		GameCamera::EmptyMainCamera();
		s_activeScene = nullptr;
		s_activeScene.reset(Serial::DeserializeJsonScene(SaveSystem::LoadJsonScene(sceneName)));
		if (s_pendingLoadCameraPos)
		{
		    Editor::SetCameraTransform(s_activeScene->m_lastSavedCameraTransform);
            s_pendingLoadCameraPos = true;
		}
	}

	// void SceneManager::SetActiveScene(Scene* scene)
	// {
	// 	Editor::selectedEntity = nullptr;
	// 	s_activeScene.reset(scene);
	// }

	void SceneManager::EnqueueSceneChange(const std::string& sceneName, bool loadCameraPos)
	{
		if (s_pendingSceneChange) return;
		s_pendingSceneName = sceneName;
		s_pendingLoadCameraPos = loadCameraPos;
		s_pendingSceneChange = true;
	}

	void SceneManager::ApplyPendingSceneChange()
	{
		if (s_pendingSceneChange) ChangeActiveScene(s_pendingSceneName);
		SetSceneChange(false);
	}

	void SceneManager::UpdateScenes()
	{
		s_scenes.clear();
		for (std::string& sceneName : SaveSystem::LoadAllSavedSceneNames())
		{
			s_scenes.push_back(sceneName);
		}
	}

	void SceneManager::RemoveScene(std::string& sceneName)
	{
	    // Remove reorders the elements so that the ones with the value of sceneName end up in the end.
		// Then erase erases from the beginning of those whose value is sceneName to the end.
	    s_scenes.erase(std::remove(s_scenes.begin(),s_scenes.end(), sceneName), s_scenes.end());
	}

	bool SceneManager::CheckIfSceneAlreadyExists(std::string sceneName)
	{
		auto it = std::find(s_scenes.begin(), s_scenes.end(), sceneName);
		if (it != s_scenes.end()) return true;
		return false;
	}

	Scene* SceneManager::ActiveScene() {
		return s_activeScene.get();
	}

	void SceneManager::ResetActiveScene()
	{
	    s_activeScene = nullptr;
	}

	SceneSPtr SceneManager::CreateDefaultScene(std::string sceneName)
	{
		SceneSPtr scene = std::make_shared<Scene>(0, sceneName);
		Entity* light = scene->createEntity("globalLight");
		light->addComponent<GlobalLight>();
		light->transform().setPosition(glm::vec3(0.0f, 100.0f, 0.0f));

		Entity* fede = scene->createEntity("fede");
		fede->addComponent<ModelRenderer>()->setModel("RuamCore/Assets/Models/Fede.obj");

		Entity* camera = scene->createEntity("camera");
		camera->addComponent<GameCamera>();
		return scene;
	}


	void SceneManager::AddSceneCreator(const std::string& sceneName)
	{
		s_scenes.push_back(sceneName);
	}

	bool SceneManager::SceneChange() {
		return s_pendingSceneChange;
	}

	void SceneManager::SetSceneChange(bool state) {
		s_pendingSceneChange = state;
	}
}
