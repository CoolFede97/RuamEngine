#pragma once

#include "Object.hpp"
#include "../../RuamCore/Input/Input.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "MeshRenderer.h"

#include "Boss.h"
#include "Bullet.h"

class Shooter : public Component {
	IMPL_SIMPLE_SERIALIZE(Shooter)

	using Component::Component;
	Shooter(const nlohmann::json& j, unsigned int obj_id) : Component(obj_id) {}
	void start()
	{
		if (s_instance == nullptr) {
			s_instance = this;
		}
		else if (s_instance != this) {
			object()->removeComponent<Shooter>(); // ESTO EST� MAL
		}
		Boss::s_instance->playerTransform = &object()->transform();
	}

	void update() {
		if (Input::GetKeyDown(KeyCode::SpaceBar_Key))
		{
		    std::cout << "HOLA WACHO!\n";
			Object* object = SceneManager::ActiveScene()->newObject(0);

			Bullet* bullet = object->addComponent<Bullet>();

			bullet->object()->transform().position() = s_instance->object()->transform().position();

			bullet->m_speed = m_bulletSpeed;
			bullet->m_direction = glm::normalize(Boss::s_instance->object()->transform().position() - s_instance->object()->transform().position());
			bullet->m_radius = m_bulletRadius;

			bullet->object()->addComponent<MeshRenderer>()->SetModel(m_bulletMeshPath);
		}
	}

public:
	static Shooter* s_instance;
	float m_bulletSpeed = 0.0f;
	float m_bulletRadius = 1.0f;
	std::string m_bulletMeshPath;
};

REGISTER_COMPONENT(Shooter)
