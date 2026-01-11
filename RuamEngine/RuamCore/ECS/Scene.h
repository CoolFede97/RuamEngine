#pragma once

#include "Component.h"
#include <list>
#include <string>
#include <iostream>
#include <map>
#include <typeindex>
#include <vector>
#include <memory>

namespace RuamEngine
{
	class Entity;
	class Component;
	class Scene {
	public:
	    Scene() : m_id(s_idCount++), m_name(s_defaultName) {std::cout << "SCENE " << m_name << " WITH ID " << m_id << " CREATED\n";;}

	    Scene(const int id) : m_id(id), m_name(s_defaultName) {std::cout << "SCENE " << m_name << " WITH ID " << m_id << " CREATED\n";;}

	    Scene(const std::string& name) : m_id(s_idCount++), m_name(name) {std::cout << "SCENE " << m_name << " WITH ID " << m_id << " CREATED\n";;}

	    Scene(const int id, const std::string& name) : m_id(id), m_name(name) {std::cout << "SCENE " << m_name << " WITH ID " << m_id << " CREATED\n";;}

	    ~Scene() {std::cout << "SCENE " << m_name << " WITH ID " << id() << " DESTROYED\n";}

		unsigned int id() const {
		    return m_id;
	    }

		const std::string& name() const { return m_name; }
	    std::string name() { return m_name; }

	    Entity* createEntity();
	    Entity* createEntity(const std::string& name);
	    Entity* createEntity(unsigned int idx);

	    Entity* getEntityByIdx(unsigned int idx) const;
	    Entity* getEntityById(unsigned int id) const;

		std::list<Entity*> getEntities() const;

	    void deleteEntityByIdx(unsigned int idx);
	    void deleteEntityById(unsigned int idx);

		inline std::map<unsigned int, std::map<std::type_index, std::vector<Component*>>>& justCreatedComponents()
		{
			return m_justCreatedComponents;
		}

		void update();

	private:

	// Key of the object that owns the component, then the type of component and you get a vector of components
	    std::map<unsigned int, std::map<std::type_index, std::vector<Component*>>> m_componentsToStart;
	    std::map<unsigned int, std::map<std::type_index, std::vector<Component*>>> m_justCreatedComponents;

	    std::list<std::unique_ptr<Entity>> m_entities;
		const std::string m_name;
	    const unsigned int m_id;
	    static unsigned int s_idCount;

		static const std::string s_defaultName;
	};
	using SceneSPtr = std::shared_ptr<Scene>;
}
