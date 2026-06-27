#pragma once

#include <iostream>

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "RenderingCore.h"
#include "Material.h"
#include "EditorCamera.h"

namespace RuamEngine
{
	class ShaderProgram
	{
		// caching for uniforms
	public:

		// Los paths son relativos a la carpeta RuamEngine
		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		~ShaderProgram();

		void bind() const;
		void unbind() const;

		// Getters
		inline std::string name() { return m_name; }
		inline std::string vertexShaderPath() { return m_vertexShaderPath; }
		inline std::string fragmentShaderPath() { return m_fragmentShaderPath; }

		// Set Uniforms
		void setUniform1i(const std::string& name, int value);
		void setUniform1f(const std::string& name, float value);
		void setUniform3f(const std::string& name, float v0, float v1, float v2);
		void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void setUniformMat4f(const std::string& name, glm::mat4 matrix);
		void setUniformTextureSlots(const std::string& name);
		void loadMaterial(Material* material);
		void updateCameraMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
		unsigned int maxTexturesCapacity() const { return  maxTextureSlots; }
		unsigned int instanceId() const { return m_instanceId; }
		unsigned int glName() const { return m_glName; }

	private:
		unsigned int compileShader(unsigned int type, const std::string& source);
		unsigned int createProgram(const std::string& vertexShader, const std::string& fragmentShader);
		int getUniformLocation(const std::string& name);
		unsigned int m_glName;
		unsigned int m_instanceId;
		unsigned int static s_idInstanceCount;
		std::unordered_map<std::string, int> m_UniformLocationCache;
		std::string m_vertexShaderPath;
		std::string m_fragmentShaderPath;
		std::string m_name; // the union of all shader paths
		static GLint maxTextureSlots;
	};

	using ShaderProgramSPtr = std::shared_ptr<ShaderProgram>;
	using ShaderProgramWPtr = std::weak_ptr<ShaderProgram>;

	using ShaderProgramName = std::string; // the union of all shader paths

}
