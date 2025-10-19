#pragma once

#include <vector>
#include <Vertex.h>
namespace RuamEngine 
{
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		MaterialPtr m_material;

		void SetUpMesh();
	};
}