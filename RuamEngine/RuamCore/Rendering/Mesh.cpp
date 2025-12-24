#include "Mesh.h"
#include "ResourceManager.h"
#include "ResourceManager.h"

namespace RuamEngine
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialPtr material)
		: m_vertices(vertices), m_indices(indices), m_material(material)
	{

	}

	Mesh::~Mesh()
	{
		ResourceManager::DestroyMaterial(m_material->GetId());
	}

	void Mesh::SetUpMesh()
	{

	}
}
