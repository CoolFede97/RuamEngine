#pragma once

#include <list>
#include <string>
#include "Object.hpp"

class Scene {
public:
    Scene() : m_id(s_id_count++), m_name(s_default_name) {}

    Scene(const int id) : m_id(id), m_name(s_default_name) {}

    Scene(const std::string& name) : m_id(s_id_count++), m_name(name) {}

    Scene(const int id, const std::string& name) : m_id(id), m_name(name) {}

	unsigned int id() const {
	    return m_id;
    }

	const std::string& name() const;

    Object* newObject(); //Maybe should need a name?
    Object* newObject(unsigned int idx);

    Object* getObjectByIdx(unsigned int idx) const;
    Object* getObjectById(unsigned int id) const;

	std::list<Object*> getObjects() const { return m_objects; }

    void deleteObjectByIdx(unsigned int idx);
    void deleteObjectById(unsigned int idx);

    std::string name() { return m_name; }

	void start();
	void update();

private:
    std::list<Object*> m_objects;
	const std::string m_name;
    const unsigned int m_id;
    static unsigned int s_id_count;

	static const std::string s_default_name;
};
using ScenePtr = std::shared_ptr<Scene>;

