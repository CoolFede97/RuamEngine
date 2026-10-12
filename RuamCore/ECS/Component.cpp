#include "Component.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Scene.h"
namespace RuamEngine
{
	unsigned int Component::s_idCount = 0;

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
	    return m_entity;
	}

	bool Component::disabledManually() const
	{
	    return m_disabledManually;
	}

	std::vector<Component*> Component::getComponents() const
	{
	    return entity()->getComponents();
	}

	Transform* Component::transform() const
	{
		return entity()->transform();
	}

	void Component::setEnabled(bool state)
	{
        if (state && !m_entity->enabled())
        {
            std::cerr << "Error: Trying to enable component of name " << name() << " on entity of name " << entity()->name() << ", but this entity is disabled!\n";
            return;
        }
        m_disabledManually = !state;
		m_enabled = state;
	}

	void Component::setEnabledByEntity(bool entityEnabled)
	{
	    m_enabled = entityEnabled && !m_disabledManually;
	}

	void Component::drawSerializedMembers()
	{
	    for (FieldInfo field : fields())
		{
		    Editor::DrawMemberInInspector(field.name, field.type, field.value, field.callbackOnChange);
		}
	}
	DEF_REGISTER_COMPONENT(Component)
}
