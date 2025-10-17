#include "SceneManager.hpp"
#include "Serial.hpp"

SceneManager::SceneList SceneManager::s_scenes;
SceneManager::ScenePtr SceneManager::s_active_scene = nullptr;

const SceneManager::SceneList& SceneManager::sceneList() {
	return s_scenes;
}

void SceneManager::SetActiveScene(const unsigned int id) {
	s_active_scene.reset(s_scenes[id]());
}

Scene* SceneManager::ActiveScene() {
	return s_active_scene.get();
}

void SceneManager::AddScene(const unsigned int id, const std::string& scene) {

	s_scenes[id] = [scene]() {
		return Serial::deserialise(scene);
	};
}

void SceneManager::RemoveScene(const int id) {
	s_scenes.erase(id);
}

SceneManager::ScenePtr SceneManager::EmptyScene() {
	return std::move(std::make_unique<Scene>());
}

void SceneManager::CreateScene(const unsigned int id, const std::string& name) {
	Scene *s = new Scene(name);
	Serial::serialise(s);
	AddScene(id, name);
	SetActiveScene(id);
	delete s;
}

bool SceneManager::StartScene(const unsigned int id, const std::string& name) {
	auto s = Serial::deserialise(name);
	bool ret = s == nullptr;
	if (ret){
		CreateScene(id, name);
	}
	else {
		AddScene(id, name);
		SetActiveScene(id);
	}
	delete s;
	return ret;
}