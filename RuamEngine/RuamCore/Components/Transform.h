#pragma once

#include "Component.hpp"
#include "glm/glm.hpp"
#include <list>

class Transform : public Component {
public:
	using Component::Component;

	Transform(unsigned int obj_id);
	Transform(unsigned int obj_id, glm::vec3 pos);

	void start();
	void update();

	void setPosition(glm::vec3 new_pos);
	void setPosition(float x, float y, float z);
	void setRotation(glm::vec3 new_rot);
	void setRotation(float x, float y, float z);

	void setScale(glm::vec3 new_pos);
	void setScale(float x, float y, float z);

	void setParent(Transform* parent);
	void addChild(Transform* child);
	void removeChild(unsigned int idx);
	const std::list<Transform*>& children() const;

	glm::vec3& position();
	glm::vec3& scale();
	glm::vec3& rotation();

	Transform* operator[](size_t idx);

protected:
	glm::vec3 m_position;
	glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f,0.0f,0.0f);
	std::list<Transform*> m_children;
};
