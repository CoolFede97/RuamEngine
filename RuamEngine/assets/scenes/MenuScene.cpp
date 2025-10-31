#include "Object.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "../components/Counter.h"
#include "AudioSource.h"
#include "Camera.h"
#include "Serial.hpp"

namespace RuamEngine
{
	void CreateMenuScene()
	{
		SceneManager::CreateScene(0, "MenuScene");
		auto menuScene = SceneManager::ActiveScene();
		Object* manager = menuScene->newObject();
		manager->addComponent<Camera>();
		manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
		manager->addComponent<Counter>();
		manager->addComponent<Manager>();
		manager->addComponent<Counter>();
		std::cout << "YAY\n";
	}
}
