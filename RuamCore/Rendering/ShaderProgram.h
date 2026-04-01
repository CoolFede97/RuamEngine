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
	enum ShaderProgramType
    {
        general = 0,
        skybox = 1
    };

	class ShaderProgram
	{
		// caching for uniforms
	public:

		// Los paths son relativos a la carpeta RuamEngine
		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
		~ShaderProgram();

		void bind() const;
		void unbind() const;

		// Set Uniforms
		void setUniform1i(const std::string& name, int value);
		void setUniform1f(const std::string& name, float value);
		void setUniform3f(const std::string& name, float v0, float v1, float v2);
		void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void setUniformMat4f(const std::string& name, glm::mat4 matrix);
		void setUniformTextureSlots(const std::string& name);
		void loadMaterial(const Material& material);
		void updateCameraMatrices();
		unsigned int maxTexturesCapacity() { return  maxTextureSlots; }
		unsigned int instanceId() const { return m_instanceId; }
		unsigned int rendererID() const { return m_glName; }

	private:
		unsigned int compileShader(unsigned int type, const std::string& source);
		unsigned int createProgram(const std::string& vertexShader, const std::string& fragmentShader);
		int getUniformLocation(const std::string& name);
		unsigned int m_glName;
		unsigned int m_instanceId;
		unsigned int static s_idInstanceCount;
		std::unordered_map<std::string, int> m_UniformLocationCache;
		std::string m_vFilePath;
		std::string m_fFilePath;
		static GLint maxTextureSlots;
	};

	using ShaderProgramSPtr = std::shared_ptr<ShaderProgram>;
}
