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
	    ResourceManager::RemoveMaterialIfExpired(m_id);
	}
}
