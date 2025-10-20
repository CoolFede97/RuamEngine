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
#include "Serial.hpp"
#include "MeshRenderer.h"

void CreateCFSandboxScene()
{
	SceneManager::CreateScene(1, "SandboxScene");
	auto sandboxScene = SceneManager::ActiveScene();
	Object* manager = sandboxScene->newObject();
	//manager->addComponent<SandboxCom>();
	Object* bag = sandboxScene->newObject();
	bag->transform().setPosition(glm::vec3(0.0f, 0.0f, 5.5f));
	bag->transform().setRotation(glm::vec3(45.0f, 5.0f, 45.0f));
	bag->addComponent<MeshRenderer>();
	bag->getComponent<MeshRenderer>()->m_meshPath = "assets/meshes/backpack/backpack.obj";
	manager->addComponent<Camera>();
	//manager->addComponent<CameraController>();
	manager->addComponent<Manager>();
	//manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
	Serial::serialise(sandboxScene);
}
