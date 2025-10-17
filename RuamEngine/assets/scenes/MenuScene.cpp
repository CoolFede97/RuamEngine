#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "../components/Counter.h"
#include "AudioSource.h"
#include "Camera.h"

namespace RuamEngine
{
	Scene* CreateMenuScene()
	{
		Scene* menuScene = SceneManager::CreateScene(0, "MenuScene");
		Object* manager = menuScene->newObject();
		manager->addComponent<Camera>();
		//manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
		manager->addComponent<Counter>();
		manager->addComponent<Manager>();
		return menuScene;
	}
}
