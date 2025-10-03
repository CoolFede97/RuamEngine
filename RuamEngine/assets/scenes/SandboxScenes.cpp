#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "../components/Counter.h"
#include "../components/SandboxCom.h"
#include "AudioSource.h"
#include "../components/SandboxCom2.h"


SceneManager::ScenePtr CreateSandboxScene()
{
	SceneManager::ScenePtr testScene = std::make_shared<Scene>("Sandbox Scene");
	Object& manager = testScene->newObject();
	manager.addComponent<SandboxCom>();
	manager.addComponent<Manager>();
	manager.addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
	return testScene;
}

SceneManager::ScenePtr CreateSandboxScene2()
{
	SceneManager::ScenePtr testScene = std::make_shared<Scene>("Sandbox Scene 2");
	Object& manager = testScene->newObject();
	manager.addComponent<SandboxCom2>();
	manager.addComponent<Manager>();
	return testScene;
}