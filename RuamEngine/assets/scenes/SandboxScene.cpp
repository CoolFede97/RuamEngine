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
#include "GlobalLight.h"

void CreateCFSandboxScene()
{
	SceneManager::CreateScene(1, "SandboxScene");
	auto sandboxScene = SceneManager::ActiveScene();

	Object* light = sandboxScene->newObject();
	light->addComponent<GlobalLight>();
	light->transform().setPosition(glm::vec3(0.0f, 0.0f, 80.0f));
	//manager->addComponent<SandboxCom>();
	//bag->transform().setRotation(glm::vec3(45.0f, 5.0f, 45.0f));
	//bag->addComponent<CubeRenderer>()->materialId = 0;
	
	Object* bag2 = sandboxScene->newObject();
	bag2->transform().setPosition(glm::vec3(0.0f, -1.5f, 7.0f));
	bag2->addComponent<MeshRenderer>()->m_meshPath = "assets/meshes/Nave/Nave.obj";
	//bag2->addComponent<CubeRenderer>()->materialId = 0;
	
	Object* bag = sandboxScene->newObject();
	bag->transform().setPosition(glm::vec3(0.0f, -1.5f, 15.0f));
	bag->addComponent<MeshRenderer>()->m_meshPath = "assets/meshes/skyboxes/galaxy/galaxySkybox.obj";
	bag->transform().setRotation(glm::vec3(180.0f, 0.0, 0.0));
	bag->transform().setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	Object* manager = sandboxScene->newObject();
	manager->addComponent<Camera>();
	manager->addComponent<CameraController>();
	manager->getComponent<CameraController>()->m_speed = 10.5f;
	manager->getComponent<CameraController>()->m_rotationSpeed = 80.0f;
	//manager->addComponent<CameraController>();
	manager->addComponent<Counter>();
	manager->addComponent<Manager>();
	//manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
	manager->addComponent<Counter>();
}
