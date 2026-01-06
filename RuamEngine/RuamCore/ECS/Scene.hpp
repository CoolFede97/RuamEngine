#pragma once

#include "Entity.hpp"
#include <list>
#include <string>
#include <iostream>

namespace RuamEngine
{
	class Scene {
	public:
	    Scene() : m_id(s_instanceCount++), m_name(s_defaultName) {std::cout << "SCENE " << m_name << " WITH ID " << m_id << " CREATED\n";;}

	    Scene(const int id) : m_id(id), m_name(s_defaultName) {std::cout << "SCENE " << m_name << " WITH ID " << m_id << " CREATED\n";;}

	    Scene(const std::string& name) : m_id(s_instanceCount++), m_name(name) {std::cout << "SCENE " << m_name << " WITH ID " << m_id << " CREATED\n";;}

	    Scene(const int id, const std::string& name) : m_id(id), m_name(name) {std::cout << "SCENE " << m_name << " WITH ID " << m_id << " CREATED\n";;}

	    ~Scene() {std::cout << "SCENE " << m_name << " WITH ID " << id() << " DESTROYED\n";}

		unsigned int id() const {
		    return m_id;
	    }

		const std::string& name() const { return m_name; }
	    std::string name() { return m_name; }

	    Entity* createEntity(); //Maybe should need a name?
	    Entity* createEntity(unsigned int idx);

	    Entity* getEntityByIdx(unsigned int idx) const;
	    Entity* getEntityById(unsigned int id) const;

		const std::list<const Entity*> getEntities() const;

	    void deleteEntityByIdx(unsigned int idx);
	    void deleteEntityById(unsigned int idx);

		void update();

		// Key of the object that owns the component, then the type of component and you get a vector of components
	    std::map<unsigned int, std::map<std::type_index, std::vector<Component*>>> m_componentsToStart;
	    std::map<unsigned int, std::map<std::type_index, std::vector<Component*>>> m_justCreatedComponents;
	private:
	    std::list<std::unique_ptr<Entity>> m_objects;
		const std::string m_name;
	    const unsigned int m_id;
	    static unsigned int s_instanceCount;

		static const std::string s_defaultName;
	};
	using SceneSPtr = std::shared_ptr<Scene>;
}
