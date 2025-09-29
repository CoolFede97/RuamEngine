#pragma once

#include <iostream>

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "RenderingCore.h"
#include "Material.h"

namespace RuamEngine
{
	class Shader
	{
		// caching for uniforms
	public:
		enum PipelineType
		{
			Generic = 0
		};

		// Los paths son relativos a la carpeta RuamEngine
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		// Set Uniforms
		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, glm::mat4 matrix);
		void LoadMaterial(const Material& material);

	private:
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		int GetUniformLocation(const std::string& name);
	
		unsigned int m_RendererID;
		PipelineType m_pipelineType;
		std::unordered_map<std::string, int> m_UniformLocationCache;
		std::string m_vFilePath;
		std::string m_fFilePath;
	};

	using ShaderPtr = std::shared_ptr<Shader>;
}


