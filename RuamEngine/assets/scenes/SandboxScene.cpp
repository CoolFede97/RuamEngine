#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "../components/Counter.h"
#include "../components/SandboxCom.h"
#include "../components/CubeRenderer.h"
#include "AudioSource.h"

Scene* CreateCFSandboxScene()
{
	Scene* testScene = SceneManager::CreateScene(1, "SandboxScene");
	Object* manager = testScene->newObject();
	manager->addComponent<SandboxCom>();
	manager->addComponent<CubeRenderer>();
	manager->addComponent<Manager>();
	//manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
	return testScene;
}
