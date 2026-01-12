#include "Scene.h"
#include "SceneManager.h"
#include "Entity.h"
#include "Component.h"

namespace RuamEngine
{

	unsigned int Scene::s_idCount = 0;
	const std::string Scene::s_defaultName = "Sample Scene";

	Entity* Scene::createEntity()
	{
		std::unique_ptr<Entity> entity = std::make_unique<Entity>();
		Entity* entity_ptr = entity.get();
	    m_entities.push_back(std::move(entity));
	    return entity_ptr;
	}

	Entity* Scene::createEntity(const std::string& name)
	{
		std::unique_ptr<Entity> entity = std::make_unique<Entity>();
		entity->setName(name);
		Entity* entity_ptr = entity.get();
	    m_entities.push_back(std::move(entity));
	    return entity_ptr;
	}

	Entity* Scene::createEntity(unsigned int idx)
	{
	    // Going to have to check this
	    //assert(idx < m_entities.size());
		std::unique_ptr<Entity> entity = std::make_unique<Entity>();
	    auto index = m_entities.cbegin();
	    std::advance(index, idx);
		Entity* entity_ptr = entity.get();
	    m_entities.insert(index, std::move(entity));
		return entity_ptr;
	}

	Entity* Scene::getEntityByIdx(const unsigned int idx) const
	{
	    auto iter = m_entities.begin();
	    return std::next(iter, idx)->get();
	}

	Entity* Scene::getEntityById(unsigned int id) const
	{
	    auto entity = std::find_if(m_entities.begin(), m_entities.end(), [id](const std::unique_ptr<Entity>& o) { return o->id() == id; });
	    if (entity == m_entities.end()) {
	        return nullptr;
	    }
	    return entity->get();
	}

	void Scene::deleteEntityByIdx(unsigned int idx)
	{
	    m_entities.erase(std::next(m_entities.begin(), idx));
	}

	void Scene::update()
	{
		m_entities.erase(
			std::remove_if(m_entities.begin(), m_entities.end(), [](std::unique_ptr<Entity>& e){ return e->destroyFlag(); }),
			m_entities.end()
		);

		m_componentsToStart = m_justCreatedComponents;

		if (m_justCreatedComponents.size()>0) m_justCreatedComponents.clear();

		for (auto& [entityId, map] : m_componentsToStart)
		{
			for (auto& [componentType, vector] : map)
			{
				for (auto& cmp : vector)
				{
					if (SceneManager::SceneChange()) return;

					if (cmp == nullptr)
					{
						std::cerr << "Error: One component from m_justCreatedComponents is null! (entity id: " << entityId << "), (Component type: " << componentType.name() << ")\n";
						continue;
					}
					if (cmp->destroyFlag()) continue;
					if (!cmp->enabled()) continue;
					cmp->start();
					cmp->markNotCreatedOnThisFrame();
				}
			}
		}
		if (m_componentsToStart.size()>0) m_componentsToStart.clear();

		for (auto& entity : m_entities)
		{
			if (SceneManager::SceneChange())
			{
				return;
			}
			if (entity == nullptr)
			{
				std::cerr << "Error: One entity from m_entities is null!\n";
				continue;
			}
			if (entity->destroyFlag()) continue;

			if (!entity->isEnabled()) continue;
			entity->update();
		}
	}

	std::list<Entity*> Scene::getEntities() const
	{
		std::list<Entity*> newEntities;
		for(auto& entity : m_entities) {
			newEntities.push_back(entity.get());
		}
		return newEntities;
	}
}
