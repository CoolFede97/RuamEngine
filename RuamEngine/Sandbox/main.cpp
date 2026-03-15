#include <iostream>
#include <memory>
#include "SaveSystem.h"
#include "SceneManager.h"
#include "Serial.h"
#include "nlohmann/json.hpp"
#include "glm/glm.hpp"
#include "Transform.h"
#include "Scene.h"
#include "Entity.h"

using namespace RuamEngine;

int main()
{
	SceneSPtr scene = std::make_shared<Scene>("TestScene");
	// SceneManager::SetActiveScene(scene.get());
	Entity* player = scene->createEntity("Player");
	player->transform().setPosition(1,2,3);
	Json jsonScene = Serial::Serialize(scene.get());

	std::cout << "Scene in JSON: \n" << jsonScene.dump(2) << "\n";

	Scene* loadedScene = Serial::DeserializeJsonScene(SaveSystem::LoadJsonScene(scene->name()));
	std::cout << "Y Pos: " << loadedScene->getEntities().front()->transform().position().y << "\n";
}
