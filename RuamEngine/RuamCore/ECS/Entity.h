#pragma once

#include <vector>
#include <memory>
#include <map>
#include <typeindex>
#include <string>
#include <algorithm>

#include "nlohmann/json.hpp"

#include "SceneManager.h"
#include "Scene.h"
#include "Component.h"
namespace RuamEngine
{
	class Transform;

	class Entity {
	public:
		Entity(const std::string& name);
		Entity() : Entity(s_defaultName) {
		}
		~Entity();

		using ComponentVector = std::vector<std::unique_ptr<Component>>;
		using ComponentListMap = std::map<std::type_index, ComponentVector>;

		void addCompToJustCreatedComponents(std::type_index tidx);

		template<class Comp>
		Comp* addComponent() {
			std::unique_ptr<Comp> comp = std::make_unique<Comp>(m_id);
			const std::type_index tidx = typeid(Comp);
			if (m_components.count(tidx) <= 0) m_components.insert({tidx, ComponentVector()});

			m_components[tidx].push_back(std::move(comp));
			addCompToJustCreatedComponents(tidx);

			return dynamic_cast<Comp*>(m_components[tidx].back().get());
		}

		template<class Comp, typename... Args>
		Comp* addComponent(Args&&... args) {
			std::unique_ptr<Comp> comp = std::make_unique<Comp>(m_id, std::forward<Args>(args...)...);
			const std::type_index tidx = typeid(Comp);
			if (m_components.count(tidx) <= 0) m_components.insert({tidx, ComponentVector()});

			m_components[tidx].push_back(std::move(comp));
			addCompToJustCreatedComponents(tidx);

			return dynamic_cast<Comp*>(m_components[tidx].back().get());
		}

		template<class Comp>
		Component* addComponentWithJson(const nlohmann::json& j) {
			std::unique_ptr<Comp> comp = std::make_unique<Comp>(j, m_id);
			const std::type_index tidx = typeid(Comp);
			if (m_components.count(tidx) > 0) m_components.insert({tidx, ComponentVector()});

			m_components[tidx].push_back(std::move(comp));
			addCompToJustCreatedComponents(tidx);

			return m_components[tidx].back().get();
		}

		// Returns ptr because a ref can't be null
		// Returned pointer is non-owning
		// TODO: Find if there's a better way
		template<class Comp>
		Comp* getComponent() const {
			auto pair = m_components.find(typeid(Comp));
			if (pair == m_components.end()) {
				return nullptr;
			}
			if (pair->second.size() == 0) {
				return nullptr;
			}
			return dynamic_cast<Comp*>(pair->second.front().get());
		}

		std::vector<Component*> getComponents() const;
		bool operator==(const Entity& obj) {
			return this->m_id == obj.m_id;
		}

		template<class Comp>
		std::vector<Comp*> getComponentsOfType() const
		{
			std::vector<Component*> comps;
			auto pair = m_components.find(typeid(Comp));
			for (auto& cmp : pair->second)
			{
				comps.push_back(cmp.get());
			}
			return comps;
		}

		// std::map<unsigned int, std::map<std::type_index, std::vector<Component*>>>& justCreatedComponents();

		// // Shouldn't be used by the user
		// template <class Comp>
		// void removeComponentInJustCreatedComponents()
		// {
		// 	auto& justCreatedEntities = Scene::m_justCreatedComponents;

		// 	auto idIt = justCreatedEntities.find(m_id);
		// 	if (idIt == justCreatedEntities.end()) return;

		// 	auto compType = idIt->second.find(typeid(Comp));
		// 	if (compType ==  idIt->second.end()) return;

		// 	if (justCreatedEntities.size()>0) compType->second.front()->destroy();
		// }

		// // Shouldn't be used by the user
		// template <class Comp>
		// void removeComponentInJustCreatedComponents(Component& comp)
		// {
		// 	auto& justCreatedEntities = Scene::m_justCreatedComponents;

		// 	auto idIt = justCreatedEntities.find(m_id);
		// 	if (idIt == justCreatedEntities.end()) return;

		// 	auto compType = idIt->second.find(typeid(Comp));
		// 	if (compType ==  idIt->second.end()) return;

		// 	auto cmp = std::find(compType->second.begin(), compType->second.end(), &comp);
		// 	if (cmp != compType->second.end()) compType->second.front()->destroy();
		// 	else std::cerr << "Couldn't find and destroy a component of type " << typeid(Comp).name() <<" with id " << comp.id() << " in the entity called" << m_name << "with id " << m_id << "\n";
		// }

		template<class Comp>
		void removeComponent() {
			auto pair = m_components.find(typeid(Comp));
			if (pair == m_components.end() || pair->second.size() == 0)
			{
				std::cerr << "Couldn't find and destroy a component of type " << typeid(Comp).name() <<" with id " << " in the entity called" << m_name << "with id " << m_id << "\n";
				return;
			}
			pair->second.front()->destroy();
			// removeComponentInJustCreatedComponents<Comp>();
		}

		template<class Comp>
		void removeComponent(Component& comp) {
			auto pair = m_components.find(typeid(Comp));
			if (pair == m_components.end()) return;
			if (pair->second.size() == 0) return;
			auto cmp = std::find(pair->second.begin(), pair->second.end(), comp);
			if (cmp != pair->second.end()) cmp->get()->destroy();
			else std::cerr << "Couldn't find and destroy a component of type " << typeid(Comp).name() <<" with id " << comp.id() << " in the entity called" << m_name << "with id " << m_id << "\n";
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

		Transform& transform();

		const Transform& transform() const;
	private:

		void forEachActiveComponent(std::function<void(Component*)> fn);

		unsigned int m_id;
		static unsigned int s_idCount;
	    std::string m_name;
		Transform* m_transform = nullptr;

		ComponentListMap m_components;

		bool m_enabled = true;
		bool m_destroyFlag = false;

		static const std::string s_defaultName;
	};
}
