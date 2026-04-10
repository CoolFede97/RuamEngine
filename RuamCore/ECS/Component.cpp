#include "Component.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Scene.h"
namespace RuamEngine
{
	unsigned int Component::s_idCount = 0;
	std::string m_forEachSerializedField = "";

	bool Component::operator==(const Component& other) const {
		return this->m_id == other.m_id;
	}

	bool Component::operator==(const std::unique_ptr<Component>& other) const {
		return other != nullptr && other->m_id == this->m_id;
	}

	unsigned int Component::id() const {
		return m_id;
	}

	Entity* Component::entity() const {
		return SceneManager::ActiveScene()->getEntityById(m_entityId);
	}

	Transform& Component::transform() const
	{
		return entity()->transform();
	}
	DEF_REGISTER_COMPONENT(Component)
}
