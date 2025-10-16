#pragma once

#include "Scene.hpp"

class SceneManager {
public:
	using ScenePtr = std::unique_ptr<Scene>;
	using SceneCreator = std::function<ScenePtr()>;
    using SceneList = std::map<unsigned int, SceneCreator>;

	SceneManager() = delete;

	static const SceneList& sceneList();

	static void SetActiveScene(unsigned int id);
	static ScenePtr ActiveScene();

	/* Returns idx */
	static void AddScene(unsigned int id, SceneCreator scene);

	// static void RemoveScene(int id);

	static ScenePtr EmptyScene();

private:
	static SceneList s_scenes;
	static ScenePtr s_active_scene;
};