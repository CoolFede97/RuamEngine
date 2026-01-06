#include "SceneManager.hpp"
#include "Serial.hpp"
#include "Camera.h"
#include "Renderer.h"

namespace RuamEngine
{
	SceneManager::SceneList SceneManager::s_scenes;
	SceneManager::ScenePtr SceneManager::s_active_scene = nullptr;
	bool SceneManager::s_has_pending_scene = false;
	unsigned int SceneManager::s_pending_scene_id = -1;
	const SceneManager::SceneList& SceneManager::sceneList() {
		return s_scenes;
	}

	void SceneManager::ResetActiveScene()
	{
		RuamEngine::Camera::EmptyMainCamera();

		// This is the order. First nullptr and then reset.
		// This is because otherwise the new objects of the new scene are created before the previous scene's objects are destroyed
		s_active_scene = nullptr;
		s_scenes[s_pending_scene_id]();
	}

	void SceneManager::SetActiveScene(Scene* newScene)
	{
		s_active_scene.reset(newScene);
	}

	void SceneManager::EnqueueSceneChange(unsigned int id)
	{
		if (s_has_pending_scene) return;
		s_pending_scene_id = id;
		s_has_pending_scene = true;
	}

	void SceneManager::ApplyPendingSceneChange()
	{
		if (s_has_pending_scene) ResetActiveScene();
		SetSceneChange(false);
	}

	Scene* SceneManager::ActiveScene() {
		return s_active_scene.get();
	}

	// void SceneManager::AddScene(const unsigned int id, const std::string& scene) {

	// 	s_scenes[id] = [scene]() {
	// 		return Serial::deserialise(scene);
	// 	};
	// }

	void SceneManager::AddSceneCreator(unsigned int id, std::function<Scene*()> sceneCreator)
	{
		s_scenes[id] = sceneCreator;
	}

	void SceneManager::RemoveScene(const int id) {
		s_scenes.erase(id);
	}

	SceneManager::ScenePtr SceneManager::EmptyScene() {
		return std::move(std::make_unique<Scene>());
	}

	// void SceneManager::CreateScene(const unsigned int id, const std::string& name) {
	// 	Scene *s = new Scene(name);
	// 	Serial::serialise(s);
	// 	AddScene(id, name);
	// 	RuamEngine::Camera::EmptyMainCamera();
	// 	s_active_scene.reset(s_scenes[id]());
	// 	s_has_pending_scene = true;
	// 	delete s;
	// }

	// bool SceneManager::StartScene(const unsigned int id, const std::string& name) {
	// 	auto s = Serial::deserialise(name);
	// 	bool ret = s == nullptr;
	// 	if (ret){
	// 		CreateScene(id, name);
	// 	}
	// 	else {
	// 		AddScene(id, name);
	// 		SetActiveScene(id);
	// 	}
	// 	delete s;
	// 	return ret;
	// }
	bool SceneManager::SceneChange() {
		return s_has_pending_scene;
	}

	void SceneManager::SetSceneChange(bool state) {
		s_has_pending_scene = state;
	}
}
