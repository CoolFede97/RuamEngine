#include "Material.h"

namespace RuamEngine
{
	Material::Material(MaterialType materialType)
		: m_type(materialType)
	{
		textures.push_back(std::make_unique<Texture>("assets/sprites/defaultSprite.png"));
	}
}