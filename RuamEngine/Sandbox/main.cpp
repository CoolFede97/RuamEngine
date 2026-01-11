#include <iostream>
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
	Scene* scene = new Scene("TestScene");
	SceneManager::SetActiveScene(scene);
	Entity* player = scene->createEntity("Player");
	player->transform().setPosition(1,2,3);
	Json jsonScene = serialize(scene);

	std::cout << "Scene in JSON: \n" << jsonScene.dump(2) << "\n";

	Scene* loadedScene = deserialize(scene->name());
	std::cout << "Y Pos: " << loadedScene->getEntities().front()->transform().position().y << "\n";
}
