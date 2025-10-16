#include "Transform.h"

Transform::Transform(const unsigned int obj_id) : Component(obj_id), m_position(0, 0, 0) {}

Transform::Transform(const unsigned int obj_id, glm::vec3 pos) : Component(obj_id), m_position(pos) {}

void start() {};
void update() {};

void Transform::setPosition(glm::vec3 new_pos) {
	m_position = new_pos;
}

void Transform::setPosition(float x, float y, float z) {
	m_position = glm::vec3(x, y, z);
}

void Transform::setRotation(glm::vec3 new_rot) {
	m_rotation = new_rot;
}

void Transform::setRotation(float x, float y, float z) {
	m_rotation = glm::vec3(x, y, z);
}

glm::vec3& Transform::position() {
	return m_position;
}

glm::vec3& Transform::scale() {
	return m_scale;
}

glm::vec3& Transform::rotation() {
	return m_rotation;
}
