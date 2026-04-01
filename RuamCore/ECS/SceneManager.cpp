#include "SceneManager.h"
//#include "Serial.h"
#include "Camera.h"
#include "Editor.h"
#include "Renderer.h"
#include "Engine.h"

#include "SaveSystem.h"

#include "GlobalLight.h"
#include "SaveSystem.h"
#include "Scene.h"
#include "Skybox.h"
#include "Camera.h"

#include <memory>


namespace RuamEngine
{
	std::vector<std::string> SceneManager::s_scenes;
	SceneManager::SceneUPtr SceneManager::s_activeScene = nullptr;
	bool SceneManager::s_pendingSceneChange = false;
	std::string SceneManager::s_pendingSceneName;
	const std::vector<std::string>& SceneManager::Scenes()
	{
		return s_scenes;
	}

	void SceneManager::ChangeActiveScene(const std::string &sceneName)
	{
		RuamEngine::Camera::EmptyMainCamera();
		RuamEngine::Skybox::EmptySkybox();

		Editor::selectedEntity = nullptr;
		s_activeScene = nullptr;
		s_activeScene.reset(Serial::DeserializeJsonScene(SaveSystem::LoadJsonScene(sceneName)));
	}

	// void SceneManager::SetActiveScene(Scene* scene)
	// {
	// 	Editor::selectedEntity = nullptr;
	// 	s_activeScene.reset(scene);
	// }

	void SceneManager::EnqueueSceneChange(const std::string& sceneName)
	{
		if (s_pendingSceneChange) return;
		s_pendingSceneName = sceneName;
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

	bool SceneManager::CheckIfSceneAlreadyExists(std::string sceneName)
	{
		auto it = std::find(s_scenes.begin(), s_scenes.end(), sceneName);
		if (it != s_scenes.end()) return true;
		return false;
	}

	Scene* SceneManager::ActiveScene() {
		return s_activeScene.get();
	}

	SceneSPtr SceneManager::CreateDefaultScene(std::string sceneName)
	{
		SceneSPtr scene = std::make_shared<Scene>(0, sceneName);
		Entity* light = scene->createEntity();
		light->addComponent<GlobalLight>();
		light->transform().setPosition(glm::vec3(0.0f, 100.0f, 0.0f));
		light->setName("Light");

		Entity* skybox = scene->createEntity();
		skybox->setName("Skybox");
		skybox->transform().setScale(glm::vec3(50,50,50));
		skybox->addComponent<Skybox>();

		Entity* player = scene->createEntity();
		player->transform().setPosition(glm::vec3(50.0f, 20.0f, 160.0f));
		player->transform().setRotation(glm::vec3(0.0f, 180, 0.0f));
		player->setName("Player2");
		player->addComponent<Camera>();

		return scene;
	}


	void SceneManager::AddSceneCreator(const std::string& sceneName)
	{
		s_scenes.push_back(sceneName);
	}

	void SceneManager::RemoveScene(const std::string& sceneName)
	{
		auto it = std::find(s_scenes.begin(), s_scenes.end(), sceneName);
		if (it != s_scenes.end())
		{
			s_scenes.erase(it);
		}
	}

	bool SceneManager::SceneChange() {
		return s_pendingSceneChange;
	}

	void SceneManager::SetSceneChange(bool state) {
		s_pendingSceneChange = state;
	}
}
