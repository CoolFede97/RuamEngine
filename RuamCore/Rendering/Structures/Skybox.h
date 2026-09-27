#pragma once

#include "ShaderProgram.h"
#include "Vertex.h"
#include "Material.h"
#include "SSBO.h"
#include "VertexArray.h"
#include "Cubemap.h"
namespace RuamEngine
{
	class Skybox
	{
	public:
	    static ShaderProgramSPtr s_shaderProgram;
		static VertexArrayUPtr s_vertexArray;
	    static bool s_inited;
		static CubemapSPtr s_cubemap;
		static SSBOUPtr<MeshVertex> s_verticesSSBO;
		static SSBOUPtr<unsigned int> s_indicesSSBO;

		static std::vector<MeshVertex> s_vertices;
		static std::vector<unsigned int> s_indices;

		static void Init();
		static void ChangeCubemap(const std::string& cubemapPath);
		static void ChangeCubemap(const std::vector<std::string>& cubemapPaths);
	};
}
