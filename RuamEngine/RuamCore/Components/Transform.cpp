#include "Transform.h"

namespace RuamEngine
{
	Transform::Transform(const unsigned int obj_id) : Component(obj_id), m_position(0, 0, 0) {}

	Transform::Transform(const unsigned int obj_id, glm::vec3 pos) : Component(obj_id), m_position(pos) {}

	void Transform::start() {}
	void Transform::update() {}

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

	void Transform::setScale(glm::vec3 new_scale) {
		m_scale = new_scale;
	}

	void Transform::setScale(float x, float y, float z) {
		m_scale = glm::vec3(x, y, z);
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

	const glm::vec3& Transform::position() const {
		return m_position;
	}

	const glm::vec3& Transform::scale() const {
		return m_scale;
	}

	const glm::vec3& Transform::rotation() const {
		return m_rotation;
	}

	void Transform::setParent(Transform* parent) {
		parent->addChild(this);
	}

	void Transform::addChild(Transform* child) {
		auto child_search = std::find(m_children.begin(), m_children.end(), child);
		if (*child_search != m_children.back()) {
			throw std::logic_error("Repeated children");
			return;
		}
		m_children.push_back(child);
	}

	void Transform::removeChild(unsigned int idx) {
		auto iter = m_children.begin();
		std::advance(iter, idx);
		m_children.erase(iter);
	}

	const std::list<Transform*>& Transform::children() const {
		return m_children;
	}

	Transform* Transform::operator[](size_t idx) {
		if (idx >= m_children.size()) {
			throw std::out_of_range("Child out of range");
		}
		auto iter = m_children.begin();
		std::advance(iter, idx);
		return *iter;
	}
}
