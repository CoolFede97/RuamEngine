#include "Scene.hpp"
#include "SceneManager.hpp"
#include <fstream>

unsigned int Scene::s_instanceCount = 0;
const std::string Scene::s_defaultName = "Sample Scene";

Object* Scene::newObject()
{
	std::unique_ptr<Object> obj = std::make_unique<Object>();
	Object* obj_ptr = obj.get();
    m_objects.push_back(std::move(obj));
    return obj_ptr;
}

Object* Scene::newObject(unsigned int idx)
{
    // Going to have to check this
    //assert(idx < m_objects.size());
	std::unique_ptr<Object> obj = std::make_unique<Object>();
    auto index = m_objects.cbegin();
    std::advance(index, idx);
	Object* obj_ptr = obj.get();
    m_objects.insert(index, std::move(obj));
	return obj_ptr;
}

Object* Scene::getObjectByIdx(const unsigned int idx) const
{
    auto iter = m_objects.begin();
    return std::next(iter, idx)->get();
}

Object* Scene::getObjectById(unsigned int id) const
{
    auto obj = std::find_if(m_objects.begin(), m_objects.end(), [id](const std::unique_ptr<Object>& o) { return o->id() == id; });
    if (obj == m_objects.end()) {
        return nullptr;
    }
    return obj->get();
}

void Scene::deleteObjectByIdx(unsigned int idx)
{
    m_objects.erase(std::next(m_objects.begin(), idx));
}

void Scene::start()
{
	SceneManager::SetSceneChange(false);
	for (auto& obj : m_objects) {
		if (!obj->isEnabled()) continue;
		obj->start();
	};
}

void Scene::update()
{
	if (SceneManager::SceneChange())
	{
		SceneManager::SetSceneChange(false);
	    std::cout << "Scene change detected\n";
		start();
		return;
	}
	m_objects.remove_if([](std::unique_ptr<Object>& obj) {return obj->destroyFlag() || obj == nullptr;});
	for (auto& obj : m_objects)
	{

		if (SceneManager::SceneChange())
		{
			return;
		}
		if (obj == nullptr) continue;
		if (obj->destroyFlag()) continue;

		if (!obj->isEnabled()) continue;
		obj->update();
	}
}

const std::list<const Object*> Scene::getObjects() const
{
	std::list<const Object*> new_objects;
	for(auto& obj : m_objects) {
		new_objects.push_back(obj.get());
	}
	return new_objects;
}
