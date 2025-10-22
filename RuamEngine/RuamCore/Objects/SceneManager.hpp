#pragma once

#include "Scene.hpp"

class SceneManager {
public:
	using ScenePtr = std::unique_ptr<Scene>;
	using SceneCreator = std::function<Scene*()>;
    using SceneList = std::map<unsigned int, SceneCreator>;

	SceneManager() = delete;

	static const SceneList& sceneList();

	static void SetActiveScene(unsigned int id);
	static Scene* ActiveScene();

	static void AddScene(unsigned int id, const std::string& scene);

	static void RemoveScene(int id);

	static ScenePtr EmptyScene();

	static void CreateScene(unsigned int id, const std::string &name);
	static bool StartScene(unsigned int id, const std::string& name);

	static bool SceneChange();

	static void SetSceneChange(bool state);

private:
	static SceneList s_scenes;
	static ScenePtr s_active_scene;
	static bool s_scene_change;
};
