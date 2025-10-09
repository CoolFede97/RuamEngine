#include "Component.hpp"
#include "SceneManager.hpp"

unsigned int Component::s_id_count = 0;

bool Component::operator==(const Component& other) const {
	return this->m_id == other.m_id;
}

unsigned int Component::id() const {
	return m_id;
}

Object* Component::object() const {
	return SceneManager::ActiveScene()->getObjectById(m_object_id);
}
