#pragma once

#include <vector>
#include <memory>
#include <map>
#include <typeindex>
#include <string>
#include <algorithm>

#include "RuamUtils.h"
#include "nlohmann/json.hpp"
#include "Component.h"
#include "Scene.h"

namespace RuamEngine
{
	class Transform;
	class Scene;
	class Entity {
	public:
		Entity(const std::string& name);
		Entity() : Entity(s_defaultName) {
		}
		~Entity();

		using ComponentVector = std::vector<std::unique_ptr<Component>>;
		using ComponentVectorMap = std::map<std::type_index, ComponentVector>;

		template<class Cmp>
		Cmp* addComponent() {
		    if constexpr(Cmp::s_unique)
			{
			    if (auto* c = getComponent<Cmp>()) return c;
			}
			std::unique_ptr<Cmp> cmp = std::make_unique<Cmp>(m_id);
			const std::type_index tidx = typeid(Cmp);
			if (m_components.count(tidx) <= 0) m_components.insert({tidx, ComponentVector()});

			m_parentScene->m_componentsToStart.push_back(cmp.get());
			m_components[tidx].push_back(std::move(cmp));

			return dynamic_cast<Cmp*>(m_components[tidx].back().get());
		}

		template<class Cmp, typename... Args>
		Cmp* addComponent(Args&&... args) {
            if constexpr(Cmp::s_unique)
            {
                if (auto* c = getComponent<Cmp>()) return c;
            }
			std::unique_ptr<Cmp> cmp = std::make_unique<Cmp>(m_id, std::forward<Args>(args...)...);
			const std::type_index tidx = typeid(Cmp);
			if (m_components.count(tidx) <= 0) m_components.insert({tidx, ComponentVector()});

			m_parentScene->m_componentsToStart.push_back(cmp.get());
			m_components[tidx].push_back(std::move(cmp));

			return dynamic_cast<Cmp*>(m_components[tidx].back().get());
		}

		// Returns ptr because a ref can't be null
		// Returned pointer is non-owning
		// TODO: Find if there's a better way
		template<class Cmp>
		Cmp* getComponent() const {
			auto pair = m_components.find(typeid(Cmp));
			if (pair == m_components.end()) {
				return nullptr;
			}
			if (pair->second.size() == 0) {
				return nullptr;
			}
			return dynamic_cast<Cmp*>(pair->second.front().get());
		}

		std::vector<Component*> getComponents() const;
		bool operator==(const Entity& obj) {
			return this->m_id == obj.m_id;
		}

		template<class Cmp>
		std::vector<Cmp*> getComponentsOfType() const
		{
			std::vector<Cmp*> comps;
			auto pair = m_components.find(typeid(Cmp));
			if (pair != m_components.end())
			{
    			for (auto& cmp : pair->second)
    			{
    				comps.push_back(dynamic_cast<Cmp*>(cmp.get()));
    			}
			}
			return comps;
		}

		template<class Cmp>
		void removeComponent() {
			auto pair = m_components.find(typeid(Cmp));
			if (pair == m_components.end() || pair->second.size() == 0)
			{
				std::cerr << "Couldn't find and destroy a component of type " << typeid(Cmp).name() <<" with id " << " in the entity called" << m_name << "with id " << m_id << "\n";
				return;
			}
			pair->second.front()->destroy();
			// removeComponentInJustCreatedComponents<Comp>();
		}

		template<class Cmp>
		void removeComponent(Component& comp) {
			auto pair = m_components.find(typeid(Cmp));
			if (pair == m_components.end()) return;
			if (pair->second.size() == 0) return;
			auto cmp = std::find(pair->second.begin(), pair->second.end(), comp);
			if (cmp != pair->second.end()) cmp->get()->destroy();
			else std::cerr << "Couldn't find and destroy a component of type " << typeid(Cmp).name() <<" with id " << comp.id() << " in the entity called" << m_name << "with id " << m_id << "\n";
		}

		unsigned int id() const;
		const std::string& name() const;
		void setName(const std::string& name);

		void update();
		void renderUpdate();


		void destroy();

		void setEnabled(bool status);

		bool isEnabled() const;

		bool destroyFlag() const;

		inline Transform* transform() const { return m_transform; };

	private:

		void forEachActiveComponent(std::function<void(Component*)> fn);

		unsigned int m_id;
		static unsigned int s_idCount;
	    std::string m_name;
		Transform* m_transform = nullptr;
		Scene* m_parentScene = nullptr;
		ComponentVectorMap m_components;

		bool m_enabled = true;
		bool m_destroyFlag = false;

		static const std::string s_defaultName;

		friend class Scene;
	};

	using EntityUPtr = std::unique_ptr<Entity>;
}
