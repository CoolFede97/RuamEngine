#include "Entity.h"
#include "Component.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "../components/CameraController.h"
#include "GlobalLight.h"
#include "Skybox.h"

Scene* CreateEmptyScene()
{
	auto scene = new Scene(0, "EmptyScene");
	SceneManager::SetActiveScene(scene);

	Entity* light = scene->createEntity("Light");
	light->addComponent<GlobalLight>();

	Entity* skybox = scene->createEntity("Skybox");
	skybox->addComponent<Skybox>();

	Entity* camera = scene->createEntity("Camera");
	camera->addComponent<Camera>();
	camera->addComponent<CameraController>();
	camera->getComponent<CameraController>()->m_speed = 10.5f;
	camera->getComponent<CameraController>()->m_rotationSpeed = 80.0f;

	return scene;
}
