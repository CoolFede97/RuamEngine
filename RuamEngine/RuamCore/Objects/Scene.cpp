#include "Scene.hpp"
#include <fstream>

unsigned int Scene::s_id_count = 0;
const std::string Scene::s_default_name = "Sample Scene";

const std::string& Scene::name() const {
	return m_name;
}

Object* Scene::newObject() {
    Object *obj = new Object();
    m_objects.push_back(obj);
    return obj;
}

Object* Scene::newObject(unsigned int idx) {
    // Going to have to check this
    //assert(idx < m_objects.size());
    Object *obj = new Object();
    auto index = m_objects.cbegin();
    std::advance(index, idx);
    m_objects.insert(index, obj);
    return obj;
}

Object* Scene::getObjectByIdx(const unsigned int idx) const {
    auto iter = m_objects.begin();
    return *std::next(iter, idx);
}

Object* Scene::getObjectById(unsigned int id) const {
    auto obj = m_objects.begin();
    while ((*obj)->id() != id) {
        std::advance(obj, 1);
    }
    return *obj;
}

void Scene::deleteObjectByIdx(unsigned int idx) {
    auto obj = *std::next(m_objects.begin(), idx);
    delete obj;
    m_objects.erase(std::next(m_objects.begin(), idx));
}

void Scene::start() {
	for (auto& obj : m_objects) {
		obj->start();
	}
}

void Scene::update() {
	for (auto& obj : m_objects) {
		obj->update();
	}
}