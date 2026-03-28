#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <map>

namespace RuamEngine
{
    class Scene;
	class SceneManager {
	public:
		using SceneSPtr = std::shared_ptr<Scene>;
		using SceneUPtr = std::unique_ptr<Scene>;

		SceneManager() = delete;

		static const std::vector<std::string>& scenes();

		static void ResetActiveScene();
		static void ChangeActiveScene(const std::string &sceneName);
		// static void SetActiveScene(Scene* scene);
		static void EnqueueSceneChange(const std::string& sceneName);
		static void ApplyPendingSceneChange();

		static void UpdateScenes();

		static Scene* ActiveScene();

		static void AddScene(unsigned int id, const std::string& scene);

		static void AddSceneCreator(const std::string& sceneName);

		static void RemoveScene(const std::string& sceneName);

		static SceneSPtr CreateDefaultScene();

		static bool SceneChange();

		static SceneUPtr s_activeScene;
	private:
		static void SetSceneChange(bool state);
		static std::vector<std::string> s_scenes;
		// static bool s_scene_change;
		static bool s_pendingSceneChange;
		static std::string s_pendingSceneName;
	};
}
