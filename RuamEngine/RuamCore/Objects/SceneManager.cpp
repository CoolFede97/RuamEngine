#include "SceneManager.hpp"

SceneManager::SceneList SceneManager::s_scenes;
SceneManager::ScenePtr SceneManager::s_active_scene = nullptr;

const SceneManager::SceneList& SceneManager::sceneList() {
	return s_scenes;
}

void SceneManager::SetActiveScene(const unsigned int id) {
	s_active_scene = std::move(s_scenes[id]());
}

SceneManager::ScenePtr SceneManager::ActiveScene() {
	return std::move(s_active_scene);
}

void SceneManager::AddScene(unsigned int id, SceneCreator creator) {
	s_scenes[id] = creator;
}

// void SceneManager::RemoveScene(const int id) {
// 	s_scenes.erase(s_scenes.begin() + id);
// }

SceneManager::ScenePtr SceneManager::EmptyScene() {
	return std::move(std::make_unique<Scene>());
}
