#include "Object.hpp"
#include "Component.hpp"
#include "Renderer.h"
#include "Scene.hpp"
#include "SceneManager.hpp"
#include "../components/Manager.h"
#include "Skybox.h"
#include "../components/Counter.h"
#include "../components/CameraController.h"
#include "Serial.hpp"
#include "ModelRenderer.h"
#include "GlobalLight.h"

#include "../components/Shooter.h"
#include "../components/Boss.h"

Scene* CreateCFSandboxScene()
{
	Scene* sandboxScene = new Scene(1, "SandboxScene");

	Object* light = sandboxScene->newObject();
	light->setName("Light");
	light->addComponent<GlobalLight>();
	light->transform().setPosition(glm::vec3(0.0f, 50.0f, 0.0f));
	//manager->addComponent<SandboxCom>();
	//bag->transform().setRotation(glm::vec3(45.0f, 5.0f, 45.0f));
	//bag->addComponent<CubeRenderer>()->materialId = 0;

	Object* bossO = sandboxScene->newObject();
	bossO->setName("Boss");
	bossO->transform().setPosition(glm::vec3(0.0f, -1.5f, 7.0f));
	bossO->transform().setScale(glm::vec3(5.0f, 5.0f, 5.0f));
	ModelRenderer* bossRenderer = bossO->addComponent<ModelRenderer>();
	bossRenderer->m_shaderProgramType = ShaderProgramType::general;
	bossRenderer->setModel("assets/meshes/boss/boss.obj");
	Boss* boss = bossO->addComponent<Boss>();
	boss->m_shootingInterval = 1.0f;
	boss->m_bulletSpeed = 20.0f;
	boss->m_bulletRadius = 1.5f;
	boss->m_health = 1;
	boss->m_bulletMeshPath = "assets/meshes/bullet/bullet.obj";

	// Object* skybox = sandboxScene->newObject();
	// skybox->setName("Skybox");
	// skybox->transform().setScale(glm::vec3(50,50,50));
	// skybox->addComponent<Skybox>();

	Object* player = sandboxScene->newObject();
	player->setName("Player");
	player->transform().setPosition(0.0f, 0.0f, -50.0f);
	player->addComponent<Camera>();
	player->addComponent<CameraController>();
	player->getComponent<CameraController>()->m_speed = 10.5f;
	player->getComponent<CameraController>()->m_rotationSpeed = 80.0f;

	Shooter* shooter = player->addComponent<Shooter>();
	shooter->m_shootingInterval = 0.3f;
	shooter->m_bulletSpeed = 10.0f;
	shooter->m_bulletRadius = 0.4f;
	shooter->m_bulletMeshPath = "assets/meshes/bullet/bullet.obj";

	Object* manager = sandboxScene->newObject();
	manager->setName("Manager");
	//manager->addComponent<CameraController>();
	manager->addComponent<Manager>();
	//manager->addComponent<AudioSource>("/home/tomy/programming/ce/masmas/RuamEngine/RuamEngine/1 Crumbling Castle.wav");

	// Serial::serialise(SceneManager::ActiveScene());

	return sandboxScene;
}
