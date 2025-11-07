#pragma once

#include <iostream>

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "RenderingCore.h"
#include "Material.h"
#include "Camera.h"

namespace RuamEngine
{
	class ShaderProgram
	{
		// caching for uniforms
	public:

		// Los paths son relativos a la carpeta RuamEngine
		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		~ShaderProgram();

		void Bind() const;
		void Unbind() const;

		// Set Uniforms
		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, glm::mat4 matrix);
		void SetUniformTextureSlots(const std::string& name);
		void LoadMaterial(const Material& material);
		void UpdateCameraMatrices();
		unsigned int GetMaxTexturesCapacity() { return  maxTextureSlots; }
		unsigned int GetInstanceId() const { return m_instanceId; }
		unsigned int GetRendererID() const { return m_rendererId; }

	private:
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int CreateProgram(const std::string& vertexShader, const std::string& fragmentShader);
		int GetUniformLocation(const std::string& name);
		unsigned int m_rendererId;
		unsigned int m_instanceId;
		unsigned int static s_idInstanceCount;
		std::unordered_map<std::string, int> m_UniformLocationCache;
		std::string m_vFilePath;
		std::string m_fFilePath;
		static GLint maxTextureSlots;
	};

	using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
}


