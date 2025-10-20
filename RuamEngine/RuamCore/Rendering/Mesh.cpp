#include "Mesh.h"

RuamEngine::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, MaterialPtr material)
	: m_vertices(vertices), m_indices(indices), m_material(material)
{

}

void RuamEngine::Mesh::SetUpMesh()
{

}
