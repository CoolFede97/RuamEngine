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
	
	Object* boss = sandboxScene->newObject();
	boss->transform().setPosition(glm::vec3(0.0f, -1.5f, 7.0f));
	boss->addComponent<MeshRenderer>()->m_meshPath = "assets/meshes/Nave/Nave.obj";
	//bag2->addComponent<CubeRenderer>()->materialId = 0;
	
	Object* skybox = sandboxScene->newObject();
	skybox->transform().setPosition(glm::vec3(0.0f, -1.5f, 15.0f));
	skybox->addComponent<MeshRenderer>()->m_meshPath = "assets/meshes/skyboxes/galaxy/galaxySkybox.obj";
	skybox->transform().setRotation(glm::vec3(180.0f, 0.0, 0.0));
	skybox->transform().setScale(glm::vec3(500.0f, 500.0f, 500.0f));

	Object* player = sandboxScene->newObject();
	player->addComponent<Camera>();
	player->addComponent<CameraController>();
	player->getComponent<CameraController>()->m_speed = 10.5f;
	player->getComponent<CameraController>()->m_rotationSpeed = 80.0f;
}
