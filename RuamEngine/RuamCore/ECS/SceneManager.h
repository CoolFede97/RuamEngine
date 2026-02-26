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
		using ScenePtr = std::unique_ptr<Scene>;
		using SceneCreator = std::function<Scene*()>;
	    using SceneList = std::map<unsigned int, SceneCreator>;

		SceneManager() = delete;

		static const SceneList& sceneList();

		static void ResetActiveScene();
		static void SetActiveScene(Scene* newScene);
		static void EnqueueSceneChange(unsigned int id);
		static void ApplyPendingSceneChange();

		static Scene* ActiveScene();

		static void AddScene(unsigned int id, const std::string& scene);

		static void AddSceneCreator(unsigned int id, std::function<Scene*()> sceneCreator);

		static void RemoveScene(int id);

		static ScenePtr EmptyScene();

		static void CreateScene(unsigned int id, const std::string &name);
		// static bool StartScene(unsigned int id, const std::string& name);

		static bool SceneChange();

		static void SetSceneChange(bool state);

		static ScenePtr s_active_scene;
	private:
		static SceneList s_scenes;
		static bool s_scene_change;
		static bool s_has_pending_scene;
		static unsigned int s_pending_scene_id;
	};
}
