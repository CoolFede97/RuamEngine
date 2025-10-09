#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "../components/Counter.h"
#include "AudioSource.h"

std::shared_ptr<Scene> CreateMenuScene()
{
	std::shared_ptr<Scene> menuScene = std::make_shared<Scene>("Menu Scene");
	Object* manager = menuScene->newObject();
	manager->addComponent<Manager>();
	manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
	manager->addComponent<Counter>();
	return menuScene;
}
