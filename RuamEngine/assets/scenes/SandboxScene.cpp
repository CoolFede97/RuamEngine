#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "../components/Counter.h"
#include "../components/SandboxCom.h"
#include "CubeRenderer.h"
#include "../components/CameraController.h"
#include "AudioSource.h"

Scene* CreateCFSandboxScene()
{
	Scene* testScene = SceneManager::CreateScene(1, "SandboxScene");
	Object* manager = testScene->newObject();
	//manager->addComponent<SandboxCom>();
	Object* cube = testScene->newObject();
	cube->transform().setPosition(glm::vec3(0.0f, 0.0f, 5.5f));
	cube->transform().setRotation(glm::vec3(45.0f, 5.0f, 45.0f));
	cube->addComponent<CubeRenderer>();
	manager->addComponent<Camera>();
	//manager->addComponent<CameraController>();
	manager->addComponent<Manager>();
	//manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
	return testScene;
}
