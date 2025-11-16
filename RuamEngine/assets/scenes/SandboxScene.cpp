#include "Object.hpp"
#include "Component.hpp"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "../components/Counter.h"
#include "../components/CameraController.h"
#include "Serial.hpp"
#include "MeshRenderer.h"
#include "GlobalLight.h"

#include "../components/Shooter.h"

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
	boss->transform().setScale(glm::vec3(5.0f, 5.0f, 5.0f));
	boss->addComponent<MeshRenderer>()->SetModel("assets/meshes/boss/boss.obj");
	boss->addComponent<Boss>();

	Object* skybox = sandboxScene->newObject();
	skybox->addComponent<MeshRenderer>()->SetModel("assets/meshes/skyboxes/galaxy/galaxySkybox.obj");
	skybox->getComponent<MeshRenderer>()->m_model->m_meshes[0].m_material->m_shininess = 1000.0;
	skybox->transform().setRotation(glm::vec3(180.0f, 0.0, 0.0));
	skybox->transform().setScale(glm::vec3(500.0f, 500.0f, 500.0f));

	Object* player = sandboxScene->newObject();
	player->addComponent<Camera>();
	player->addComponent<CameraController>();
	player->getComponent<CameraController>()->m_speed = 10.5f;
	player->getComponent<CameraController>()->m_rotationSpeed = 80.0f;

	Shooter* shooter = player->addComponent<Shooter>();

	shooter->m_bulletSpeed = 10.0f;
	shooter->m_bulletRadius = 0.4f;
	shooter->m_bulletMeshPath = "assets/meshes/bullet/bullet.obj";

	Object* manager = sandboxScene->newObject();
	//manager->addComponent<CameraController>();
	manager->addComponent<Manager>();
	//manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");
}
