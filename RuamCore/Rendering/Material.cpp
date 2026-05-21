#include "Material.h"
#include "ResourceManager.h"

namespace RuamEngine
{
	unsigned int Material::s_idCount = 0;
	Material::Material()
		: m_id(s_idCount++)
	{

	}
	Material::~Material()
	{
	    auto it = ResourceManager::m_materialCache.find(m_id);
     	if (it != ResourceManager::m_materialCache.end())
        {
       		ResourceManager::m_materialCache.erase(it);
        }
	}
}
