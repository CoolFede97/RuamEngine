#include "Entity.hpp"
#include "SceneManager.hpp"


namespace RuamEngine
{
	unsigned int Entity::s_instanceCount = 0;
	const std::string Entity::s_defaultName = "GameObject";

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
		for (auto& cmp_vec : m_components)
		{
			auto tmp = std::remove_if(cmp_vec.second.begin(), cmp_vec.second.end(), [](std::unique_ptr<Component>& cmp){ return cmp->destroyFlag(); });
		}
		for (auto& pair : m_components)
		{
			for (auto& cmp : pair.second)
			{
				if (SceneManager::SceneChange()) return;
				if (destroyFlag()) return;
				if (cmp->destroyFlag()) continue;
				if (cmp->createdOnThisFrame()) continue;
				if (!cmp->enabled()) continue;

				cmp->update();
			}
	    }
	}

	void Entity::destroy()
	{
		m_destroyFlag = true;
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
		return m_transform;
	}

	const Transform &Entity::transform() const
	{
		return m_transform;
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
		// std::cout << "m_components size: " << m_components[tidx].size() << "\n";
		SceneManager::ActiveScene()->m_justCreatedComponents[m_id][tidx].push_back(m_components[tidx].back().get());
	}

	std::map<unsigned int, std::map<std::type_index, std::vector<Component*>>>& Entity::justCreatedComponents()
	{
		return SceneManager::ActiveScene()->m_justCreatedComponents;
	}
}
