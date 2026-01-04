#include "Object.hpp"
#include "SceneManager.hpp"

unsigned int Object::s_instanceCount = 0;
const std::string Object::s_defaultName = "GameObject";

Object::~Object()
{
    for (auto &comp_v: m_components)
    {
        for (auto &&comp: comp_v.second)
        {
            comp.reset();
        }
        comp_v.second.clear();
    }
}

unsigned int Object::id() const
{
    return m_id;
}

const std::string &Object::name() const
{
    return m_name;
}

void Object::setName(const std::string &name)
{
    m_name = name;
}

void Object::start()
{
	for (auto &cmp : getComponents())
	{
		if (SceneManager::SceneChange()) break;
		if (m_destroyFlag) break;
		cmp->start();
		cmp->markStartedFlag();
    }
}

void Object::update()
{
	for (auto& cmp_vec : m_components)
	{
		auto tmp = std::remove_if(cmp_vec.second.begin(), cmp_vec.second.end(), [](std::unique_ptr<Component>& cmp){ return cmp->destroyFlag(); });
	}
	for (auto &cmp: getComponents())
	{
		if (SceneManager::SceneChange()) break;
		if (m_destroyFlag) break;
		if (cmp->destroyFlag()) continue;

		// This comprobation is taken into account in case a component is added during run time
		if (!cmp->startedFlag())
		{
			cmp->start();
			cmp->markStartedFlag();
		}
		else cmp->update();
    }
}

void Object::destroy()
{
	m_destroyFlag = true;
}

bool Object::destroyFlag() const
{
	return m_destroyFlag;
}

void Object::setEnabled(bool status)
{
	m_enabled = status;
}

bool Object::isEnabled() const
{
	return m_enabled;
}

Transform &Object::transform()
{
	return m_transform;
}

const Transform &Object::transform() const
{
	return m_transform;
}

std::vector<Component*> Object::getComponents() const
{
	std::vector<Component*> comps;
	for (auto& comp_v : m_components)
	{
		for (auto& cmp : comp_v.second)
		{
	   		comps.push_back(cmp.get());
		}
	}
	return comps;
}
