#include "Entity.h"
#include "Component.h"
#include "RuamUtils.h"
#include "Transform.h"

namespace RuamEngine
{
	unsigned int Entity::s_idCount = 0;
	const std::string Entity::s_defaultName = "GameObject";

	Entity::Entity(const std::string& name)
	: m_id(s_idCount++), m_name(name)
	{
		m_transform = addComponent<Transform>();
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
		for (auto& [cmpType, cmpVector] : m_components)
		cmpVector.erase(
			std::remove_if(cmpVector.begin(), cmpVector.end(), [](std::unique_ptr<Component>& cmp){ return cmp->destroyFlag(); }),
			cmpVector.end()
		);
		forEachActiveComponent([](Component* cmp)->void {cmp->update();});
	}
	void Entity::renderUpdate()
	{
		forEachActiveComponent([](Component* cmp)->void {cmp->renderUpdate();});
	}

	void Entity::forEachActiveComponent(std::function<void(Component*)> fn)
	{
		for (auto& pair : m_components)
		{
			for (auto& cmp : pair.second)
			{
				if (SceneManager::SceneChange()) return;
				if (cmp->destroyFlag()) continue;
				if (cmp->createdOnThisFrame()) continue;
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
	}

	bool Entity::destroyFlag() const
	{
		return m_destroyFlag;
	}

	void Entity::setEnabled(bool status)
	{
		m_enabled = status;
	}

	bool Entity::isEnabled() const
	{
		return m_enabled;
	}

	Transform &Entity::transform()
	{
		return *m_transform;
	}

	const Transform &Entity::transform() const
	{
		return *m_transform;
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

	void Entity::addCompToJustCreatedComponents(std::type_index tidx)
	{
		SceneManager::ActiveScene()->justCreatedComponents()[m_id][tidx].push_back(m_components[tidx].back().get());
	}

}
