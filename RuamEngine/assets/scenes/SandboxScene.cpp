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
	bag->transform().setPosition(glm::vec3(-7.0f, 0.0f, 15.0f));
	bag->transform().setRotation(glm::vec3(45.0f, 5.0f, 45.0f));
	//bag->addComponent<CubeRenderer>();
	
	/*Object* bag2 = sandb	oxScene->newObject();
	bag2->transform().setPosition(glm::vec3(7.0f, 0.0f, 15.5f));
	bag2->transform().setRotation(glm::vec3(45.0f, 5.0f, 45.0f));
	bag2->addComponent<CubeRenderer>();*/

	bag->addComponent<MeshRenderer>();
	bag->getComponent<MeshRenderer>()->m_meshPath = "assets/meshes/backpack/backpack.obj";
	bag->addComponent<CubeRenderer>()->materialId = 0;
	manager->addComponent<Camera>();/*
	manager->addComponent<CameraController>();
	manager->getComponent<CameraController>()->m_speed = 50.0f;
	manager->getComponent<CameraController>()->m_rotationSpeed = 5000.0f;*/
	//manager->addComponent<CameraController>();
	manager->addComponent<Manager>();
	//manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
	Serial::serialise(sandboxScene);
}
