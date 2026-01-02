#include "Material.h"

namespace RuamEngine
{
	unsigned int Material::s_idCount = 0;
	Material::Material()
		: m_id(s_idCount++)
	{

	}

}
