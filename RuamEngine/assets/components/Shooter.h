#pragma once

#include "Object.hpp"
#include "../../RuamCore/Input/Input.h"

class Boss : Component {
	IMPL_SIMPLE_SERIALIZE(Boss)
	
	void start() {
		if (instance == nullptr) {
			instance = this;
		} else if (instance != this) {
			object()->removeComponent<Boss>();
		}
	}
	
	void update() {

	}
protected:
	static Boss* instance;
};
REGISTER_COMPONENT(Boss);

class Bullet : public Component {
	IMPL_SIMPLE_SERIALIZE(Bullet)

	void start() {}
	
	void update() {

	}

private:
	float speed;
	float radius;
	glm::vec3 direction;
	Transform& target_transform;
};

REGISTER_COMPONENT(Bullet)

class Shooter : public Component {
	IMPL_SIMPLE_SERIALIZE(Shooter)

	void start() {}
	
	void update() {
		if (Input::GetKeyDown(KeyCode::SpaceBar_Key)) {
			Object* object = SceneManager::ActiveScene()->newObject(0);

			object->addComponent<Bullet>();
		}
	}
};

REGISTER_COMPONENT(Shooter)
