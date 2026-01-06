#include "Scene.hpp"
#include "SceneManager.hpp"
#include <fstream>

namespace RuamEngine
{

	unsigned int Scene::s_instanceCount = 0;
	const std::string Scene::s_defaultName = "Sample Scene";

	Entity* Scene::createEntity()
	{
		std::unique_ptr<Entity> obj = std::make_unique<Entity>();
		Entity* obj_ptr = obj.get();
	    m_objects.push_back(std::move(obj));
	    return obj_ptr;
	}

	Entity* Scene::createEntity(unsigned int idx)
	{
	    // Going to have to check this
	    //assert(idx < m_objects.size());
		std::unique_ptr<Entity> obj = std::make_unique<Entity>();
	    auto index = m_objects.cbegin();
	    std::advance(index, idx);
		Entity* obj_ptr = obj.get();
	    m_objects.insert(index, std::move(obj));
		return obj_ptr;
	}

	Entity* Scene::getEntityByIdx(const unsigned int idx) const
	{
	    auto iter = m_objects.begin();
	    return std::next(iter, idx)->get();
	}

	Entity* Scene::getEntityById(unsigned int id) const
	{
	    auto obj = std::find_if(m_objects.begin(), m_objects.end(), [id](const std::unique_ptr<Entity>& o) { return o->id() == id; });
	    if (obj == m_objects.end()) {
	        return nullptr;
	    }
	    return obj->get();
	}

	void Scene::deleteEntityByIdx(unsigned int idx)
	{
	    m_objects.erase(std::next(m_objects.begin(), idx));
	}

	void Scene::update()
	{
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
						std::cerr << "Error: One component from m_justCreatedComponents is null! (Object id: " << entityId << "), (Component type: " << componentType.name() << ")\n";
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

		for (auto& obj : m_objects)
		{
			if (SceneManager::SceneChange())
			{
				return;
			}
			if (obj == nullptr)
			{
				std::cerr << "Error: One object from m_objects is null!\n";
				continue;
			}
			if (obj->destroyFlag()) continue;

			if (!obj->isEnabled()) continue;
			obj->update();
		}
	}

	const std::list<const Entity*> Scene::getEntities() const
	{
		std::list<const Entity*> new_objects;
		for(auto& obj : m_objects) {
			new_objects.push_back(obj.get());
		}
		return new_objects;
	}
}
