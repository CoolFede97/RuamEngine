#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "../components/Counter.h"
#include "AudioSource.h"

Scene* CreateMenuScene()
{
	Scene* menuScene = SceneManager::CreateScene(0, "MenuScene");
	Object* manager = menuScene->newObject();
	manager->addComponent<Manager>();
	//manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
	manager->addComponent<Counter>();
	return menuScene;
}
