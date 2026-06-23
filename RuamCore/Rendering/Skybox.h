#pragma once

#include "ShaderProgram.h"
#include "Vertex.h"
#include "Material.h"
#include "SSBO.h"
#include "VertexArray.h"
namespace RuamEngine
{
	class Skybox
	{
	    static ShaderProgramSPtr s_shaderProgram;
		static VertexArrayUPtr m_vertexArray;
	    static bool s_initialized;
		static TextureSPtr s_cubemap;
		static SSBOUPtr<Vertex> s_verticesSSBO;
		static SSBOUPtr<unsigned int> s_indicesSSBO;

		static std::vector<Vertex> s_vertices;
		static std::vector<unsigned int> s_indices;

	public:
		static void Init();
		static void ChangeCubemap(const std::string& cubemapPath);
		static void ChangeCubemap(const std::vector<std::string>& cubemapPaths);
		static void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	};
}
