#pragma once

#include <vector>
#include <memory>
#include <map>
#include <typeindex>
#include <string>
#include <algorithm>

#include "Component.hpp"
#include "Transform.h"

#include "RuamUtils.h"

class Object {
public:
	Object(const std::string& name) : m_id(s_id_count++), m_name(name), m_transform(m_id) {}
	Object() : Object(s_default_name) {
	}
	~Object();

	using ComponentVector = std::vector<std::unique_ptr<Component>>;
	using ComponentList = std::map<std::type_index, ComponentVector>;

	template<class Comp>
	Comp* addComponent() {
		EASY_FUNCTION("Add Component");
		std::unique_ptr<Comp> comp = std::make_unique<Comp>(m_id);
		const std::type_index tidx = typeid(Comp);
		if (m_components.count(tidx) > 0) {
			m_components[tidx].push_back(std::move(comp));
		} else {
			m_components.insert({tidx, ComponentVector()});
			m_components[tidx].push_back(std::move(comp));
		}
		return dynamic_cast<Comp*>(m_components[tidx].back().get());
	}

	template<class Comp, typename... Args>
	Comp& addComponent(Args&&... args) {
		EASY_FUNCTION("Add Component args")
		std::unique_ptr<Comp> comp = std::make_unique<Comp>(m_id, std::forward<Args>(args...)...);
		const std::type_index tidx = typeid(Comp);
		if (m_components.count(tidx) > 0) {
			m_components[tidx].push_back(std::move(comp));
		} else {
			m_components.insert({tidx, ComponentVector()});
			m_components[tidx].push_back(std::move(comp));
		}
		return *dynamic_cast<Comp*>(m_components[tidx].back().get());
	}

	template<class Comp>
	Comp& addComponent(const nlohmann::json& j) {
		EASY_FUNCTION("Add Component args")
		std::unique_ptr<Comp> comp = std::make_unique<Comp>(j);
		const std::type_index tidx = typeid(Comp);
		if (m_components.count(tidx) > 0) {
			m_components[tidx].push_back(std::move(comp));
		} else {
			m_components.insert({tidx, ComponentVector()});
			m_components[tidx].push_back(std::move(comp));
		}
		return *dynamic_cast<Comp*>(m_components[tidx].back().get());
	}

	template<class Comp>
	Component* addComponentPtr(const nlohmann::json& j) {
		EASY_FUNCTION("Add Component args")
		std::unique_ptr<Comp> comp = std::make_unique<Comp>(j, m_id);
		const std::type_index tidx = typeid(Comp);
		if (m_components.count(tidx) > 0) {
			m_components[tidx].push_back(std::move(comp));
		} else {
			m_components.insert({tidx, ComponentVector()});
			m_components[tidx].push_back(std::move(comp));
		}
		return m_components[tidx].back().get();
	}


	// Returns ptr because a ref can't be null
	// Returned pointer is non-owning
	// TODO: Find if there's a better way
	template<class Comp>
	Comp* getComponent() const {
		EASY_FUNCTION("Get Component")
		auto pair = m_components.find(typeid(Comp));
		if (pair == m_components.end()) {
			return nullptr;
		}
		if (pair->second.size() == 0) {
			return nullptr;
		}
		return dynamic_cast<Comp*>(pair->second.front().get());
	}

	template<class Comp>
	void removeComponent() {
		EASY_FUNCTION("Remove Component")
		auto pair = m_components.find(typeid(Comp));
		if (pair == m_components.end()) return;
		if (pair->second.size() == 0) return;
		pair->second.pop_back();
	}

	template<class Comp>
	void removeComponent(Comp& comp) {
		EASY_FUNCTION("Remove Component")
		auto pair = m_components.find(typeid(Comp));
		if (pair == m_components.end()) return;
		if (pair->second.size() == 0) return;
		std::remove(pair->second.begin(), pair->second.end(), comp);
	}

	unsigned int id() const;
	const std::string& name() const;
	void setName(const std::string& name);

	std::vector<Component*> getComponents() const;
	bool operator==(const Object& obj) {
		return this->m_id == obj.m_id;
	}

	void start();
	void update();

	void destroy();

	void setEnabled(bool status);

	bool isEnabled() const;

	bool marked_destruction() const;

	Transform& transform();

private:
	unsigned int m_id;
	static unsigned int s_id_count;
    std::string m_name;
	Transform m_transform;

	ComponentList m_components;

	bool m_enabled = true;
	bool m_destroy_flag = false;

	static const std::string s_default_name;
};
