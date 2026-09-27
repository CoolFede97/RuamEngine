#include "Material.h"
#include "ResourceManager.h"

namespace RuamEngine
{
	unsigned int Material::s_idCount = 0;
	Material::Material(Texture2DSPtr diffuse, Texture2DSPtr specular, Texture2DSPtr reflection)
		: m_id(s_idCount++), m_diffuseTexture(diffuse), m_specularTexture(specular), m_reflectionTexture(reflection)
	{

	}
	Material::~Material()
	{
	    ResourceManager::RemoveMaterialIfExpired(m_id);
	}
}
