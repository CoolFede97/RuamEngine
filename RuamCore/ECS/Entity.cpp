#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"

namespace RuamEngine
{
	unsigned int Entity::s_idCount = 1;
	const std::string Entity::s_defaultName = "GameObject";

	Entity::Entity(const std::string& name)
	: m_id(s_idCount++), m_name(name)
	{
	}

	Entity::~Entity()
	{
	    for (auto &comp_v: m_components)
	    {
	        for (auto &&comp: comp_v.second)
	        {
	            comp.reset();
	        }
	        comp_v.second.clear();
	    }
	}

	unsigned int Entity::id() const
	{
	    return m_id;
	}

	const std::string &Entity::name() const
	{
	    return m_name;
	}

	void Entity::setName(const std::string &name)
	{
	    m_name = name;
	}

	void Entity::update()
	{
		forEachActiveComponent([](Component* cmp)->void {cmp->update();});
	}
	void Entity::renderUpdate()
	{
		forEachActiveComponent([](Component* cmp)->void {cmp->renderUpdate();});
	}

	void Entity::forEachActiveComponent(std::function<void(Component*)> fn) // Should be used for native functions (such as update and renderUpdate)
	{
		for (auto& pair : m_components)
		{
    	    int componentsAmount = pair.second.size();
    		for (int cmpIndex = 0; cmpIndex < componentsAmount; cmpIndex++)
    		{
                auto& cmp = pair.second[cmpIndex];
				if (!cmp->enabled()) continue;

				fn(cmp.get());
			}
	    }
	}

	void Entity::destroy()
	{
		m_destroyFlag = true;
		for (auto& [typeIndex, cmpVector] : m_components)
		{
			for (auto& cmp : cmpVector) cmp->destroy();
		}
		if (transform()->children().size()>0)
		{
			for (auto& childTransform : transform()->children())
			{
				childTransform->entity()->destroy();
			}
		}
	}

	bool Entity::destroyFlag() const
	{
		return m_destroyFlag;
	}

	void Entity::setEnabled(bool state)
	{
	    if (state && transform()->parent() && !transform()->parent()->entity()->enabled())
		{
  		    std::cout << "Error: Trying to enable entity of name " << m_name << " with id " << id() << ", but its parent is disabled!\n";
 			return;
		}
		m_disabledManually = !state;
		setEnabledRecursive(transform(), state);
	}

	void Entity::setEnabledRecursive(Transform* childTransform, bool parentEnabled) // Used internally. Shouldn't be used by the user
	{
	    Entity* entity = childTransform->entity();
	    bool effectiveEnabled = parentEnabled && !entity->m_disabledManually;

	    entity->m_enabled = effectiveEnabled;
	    entity->setComponentsEnabled(effectiveEnabled);

	    for (Transform* transform : childTransform->children())
		{
		    setEnabledRecursive(transform, effectiveEnabled);
		}
	}

	bool Entity::enabled() const
	{
		return m_enabled;
	}

	std::vector<Component*> Entity::getComponents() const
	{
		std::vector<Component*> comps;
		for (auto& comp_v : m_components)
		{
			for (auto& cmp : comp_v.second)
			{
		   		comps.push_back(cmp.get());
			}
		}
		return comps;
	}
	void Entity::setComponentsEnabled(bool state)
	{
	    for (auto& cmpPair : m_components)
		{
		    for (auto& cmp : cmpPair.second)
			{
				cmp->setEnabledByEntity(state);
			}
		}
	}
}
