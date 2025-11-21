#include "Object.hpp"
#include "SceneManager.hpp"

unsigned int Object::s_id_count = 0;
const std::string Object::s_default_name = "Object";

Object::~Object() {
    for (auto &comp_v: m_components) {
        for (auto &&comp: comp_v.second) {
            comp.reset();
        }
        comp_v.second.clear();
    }
}

unsigned int Object::id() const {
    return m_id;
}

const std::string &Object::name() const {
    return m_name;
}

void Object::setName(const std::string &name) {
    m_name = name;
}

void Object::start() {
	for (auto &cmp : getComponents()) {
		if (SceneManager::SceneChange()) break;
		if (m_destroy_flag) break;
		cmp->start();
    }
}

void Object::update() {
	for (auto& cmp_vec : m_components) {
		auto tmp = std::remove_if(cmp_vec.second.begin(), cmp_vec.second.end(), [](std::unique_ptr<Component>& cmp){ return cmp->destruction_mark(); });
	}
	for (auto &cmp: getComponents()) {
		if (SceneManager::SceneChange()) continue;
		if (m_destroy_flag) break;
		if (cmp->destruction_mark()) continue;
        cmp->update();
    }
}

void Object::destroy() {
	m_destroy_flag = true;
}

bool Object::marked_destruction() const {
	return m_destroy_flag;
}

void Object::setEnabled(bool status) {
	m_enabled = status;
}

bool Object::isEnabled() const {
	return m_enabled;
}

Transform &Object::transform() {
	return m_transform;
}

std::vector<Component*> Object::getComponents() const {
	std::vector<Component*> comps;
	for (auto& comp_v : m_components) {
		for (auto& cmp : comp_v.second) {
	   		comps.push_back(cmp.get());
		}
	}
	return comps;
}
