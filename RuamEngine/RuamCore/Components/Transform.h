#pragma once

#include "Component.h"
#include "glm/glm.hpp"
#include "JsonConverters.h"
#include <list>

namespace RuamEngine
{
	class Entity;

	class Transform : public Component
	{
	public:
		using Component::Component;

		Transform(unsigned int entityId);
		Transform(Json transformData, const unsigned int entityId);

		void start();
		void update();

		void setPosition(glm::vec3 new_pos);
		void setPosition(float x, float y, float z);
		void setRotation(glm::vec3 new_rot);
		void setRotation(float x, float y, float z);

		void setScale(glm::vec3 new_scale);
		void setScale(float x, float y, float z);

		void setParent(Transform* parent);
		void addChild(Transform* child);
		void removeChild(unsigned int idx);
		const std::list<Transform*>& children() const;

		glm::vec3& position();
		glm::vec3& scale();
		glm::vec3& rotation();

		const glm::vec3& position() const;
		const glm::vec3& scale() const;
		const glm::vec3& rotation() const;

		Transform* operator[](size_t idx);

		std::list<Transform*> m_children;
		Transform* m_parent = nullptr; // If null, there is no parent

	protected:
		glm::vec3 m_position;
		glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 m_rotation = glm::vec3(0.0f,0.0f,0.0f);

	public:
	IMPL_SERIALIZE(Transform,
		SER_FIELD(m_position),
		SER_FIELD(m_rotation),
		SER_FIELD(m_scale))
	};
}
