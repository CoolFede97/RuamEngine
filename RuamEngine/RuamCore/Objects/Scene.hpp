#pragma once

#include <list>
#include <string>
#include "Object.hpp"

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

    Object* newObject(); //Maybe should need a name?
    Object* newObject(unsigned int idx);

    Object* getObjectByIdx(unsigned int idx) const;
    Object* getObjectById(unsigned int id) const;

	const std::list<const Object*> getObjects() const;

    void deleteObjectByIdx(unsigned int idx);
    void deleteObjectById(unsigned int idx);


	void start();
	void update();

private:
    std::list<std::unique_ptr<Object>> m_objects;
	const std::string m_name;
    const unsigned int m_id;
    static unsigned int s_instanceCount;

	static const std::string s_defaultName;
};
using SceneSPtr = std::shared_ptr<Scene>;
