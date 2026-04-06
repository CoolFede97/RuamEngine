#include "Transform.h"
#include "Component.h"
#include "Entity.h"

namespace RuamEngine
{
	Transform::Transform(const unsigned int entityId) : Component(entityId), m_position(0, 0, 0) {}
	Transform::Transform(Json transformData, const unsigned int entityId) : Component(entityId)
	{
		if (transformData.contains("m_position")) m_position = transformData["m_position"].get<glm::vec3>();
		if (transformData.contains("m_rotation")) m_rotation = transformData["m_rotation"].get<glm::vec3>();
		if (transformData.contains("m_scale")) m_scale = transformData["m_scale"].get<glm::vec3>();
	}
	Transform::~Transform()
	{
		if (m_parent!=nullptr) m_parent->m_children.remove(this);
		for (Transform* child : m_children)
		{
			if (child!=nullptr) child->m_parent = nullptr;
		}
	}
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
		if (m_parent == parent) return;

		if (m_parent != nullptr) m_parent->m_children.remove(this);
		m_parent = parent;

		if (parent != nullptr)
		{
			auto it = std::find(parent->m_children.begin(), parent->m_children.end(), this);
			if (it == parent->m_children.end())
			{
				parent->m_children.push_back(this);
			}
		}
	}

	void Transform::addChild(Transform* child) {
		if (child==nullptr)
		{
			std::cerr << "Error: Can't add child if child is nullptr!\n";
			return;
		}
		auto child_search = std::find(m_children.begin(), m_children.end(), child);
		if (child_search != m_children.end()) {
			throw std::logic_error("Repeated children");
			return;
		}
		m_children.push_back(child);
		child->setParent(this);
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
	DEF_REGISTER_COMPONENT(Transform);
}
