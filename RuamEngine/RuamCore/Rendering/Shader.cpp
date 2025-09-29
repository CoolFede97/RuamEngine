#include "Shader.h"
#include "FileFunctions.h"
#include "Renderer.h"

namespace RuamEngine
{
	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
		: m_vFilePath(GlobalizePath(vertexPath)), m_fFilePath(GlobalizePath(fragmentPath)), m_RendererID(0)
	{
		m_RendererID = CreateShader(vertexPath, fragmentPath);
	}

	Shader::~Shader()
	{

	}



	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
	
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		// The second parameter indicates how many strings will be in the array of the source
		// The second one, if it's nullptr, indicates that the strings of the array will be null terminated
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		int result;
		// glGetShaderiv returns a parameter from a shader object. In this case the parameter will be stored in result
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(id, length, &length, message));
			std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader! " << "\n";
			std::cout << message << "\n";
			GLCall(glDeleteShader(id));
			return 0;
		}

		return id;
	
	}

	unsigned int Shader::CreateShader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, RelativeFileToString(vertexPath));
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, RelativeFileToString(fragmentPath));

		// A program is a group of glsl that can run on the GPU
		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}

	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_RendererID));
	}

	void Shader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void Shader::SetUniform1i(const std::string& name, int value)
	{
		GLCall(glUniform1i(GetUniformLocation(name), value));
	}

	void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}

	void Shader::SetUniform1f(const std::string& name, float value)
	{
		GLCall(glUniform1f(GetUniformLocation(name), value));
	}

	void Shader::SetUniformMat4f(const std::string& name, glm::mat4 matrix)
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	}

	void Shader::LoadMaterial(const Material& material)
	{
		Bind();

		SetUniform4f("u_albedoColor", material.albedoColor.x, material.albedoColor.y, material.albedoColor.z, material.albedoColor.w);
		SetUniform1f("u_metallic", material.metallic);
		SetUniform1f("u_roughness", material.roughness);
		SetUniform1f("u_ambientOcclusion", material.ambientOcclusion);
		SetUniform1f("u_emissiveStrength", material.emissiveStrength);

		glm::mat4 model = glm::mat4(1.0f);
		SetUniformMat4f("u_model", model);
		SetUniformMat4f("u_view", model);
		SetUniformMat4f("u_projection", model);
		/*ASSERT(material.textures[0].m_LocalBuffer);
		ASSERT(texture.m_LocalBuffer);
		ASSERT(*material.textures[0].m_LocalBuffer == *texture.m_LocalBuffer);
		*/
		for (int i = 0 ; i < material.textures.size(); i++)
		{
			material.textures[i].Bind(i);
		}

		/*ASSERT(*texture.m_LocalBuffer == *material.textures[0].m_LocalBuffer);*/
		//material.textures[0].Bind(0);
	}	

	int Shader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location;
		GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));
		if (location == -1)
			std::cout << "Warning: uniform " << name << " does not exist!\n";
	
		m_UniformLocationCache[name] = location;
	
		return location;
	}
}

