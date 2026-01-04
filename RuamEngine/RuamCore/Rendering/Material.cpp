#include "Material.h"

namespace RuamEngine
{
	unsigned int Material::s_instanceCount = 0;
	Material::Material()
		: m_id(s_instanceCount++)
	{

	}

}
