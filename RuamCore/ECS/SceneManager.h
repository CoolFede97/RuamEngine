#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <map>
#include "SaveSystem.h"
#include "Scene.h"

namespace RuamEngine
{
	class SceneManager {
	public:
		using SceneSPtr = std::shared_ptr<Scene>;
		using SceneUPtr = std::unique_ptr<Scene>;

		SceneManager() = delete;

		static const std::vector<std::string>& Scenes();

		static void EnqueueSceneChange(const std::string& sceneName, bool loadCameraPos);
		static void CheckForSceneChange();

		static void EnqueSceneDeletion(std::string& sceneName);
		static void CheckForSceneDeletion();

		static void UpdateScenes();

		static bool CheckIfSceneAlreadyExists(std::string sceneName);

		static Scene* ActiveScene();

		static void AddScene(unsigned int id, const std::string& scene);

		static void AddSceneCreator(const std::string& sceneName);


		static bool SceneChange();
		static bool SceneDeletion();

		static SceneUPtr s_activeScene;
	private:
		static void ChangeActiveScene(const std::string &sceneName);
		static void DeleteScene();

		static SceneSPtr CreateDefaultScene(std::string sceneName = "defaultScene");

		static void ResetActiveScene(); // Makes active scene be nullptr

		static std::vector<std::string> s_scenes;
		// static bool s_scene_change;
		static bool s_pendingSceneChange;
		static bool s_pendingLoadCameraPos; // determines whether the editor's camera pos will be reseted when loading a scene
		static std::string s_pendingSceneChangeName;
		static bool s_pendingSceneDeletion;
		static std::string s_pendingSceneDeletionName;
		friend class Editor;
		friend class SaveSystem;
	};
}
