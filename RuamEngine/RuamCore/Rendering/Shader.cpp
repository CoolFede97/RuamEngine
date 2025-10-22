#include "Shader.h"
#include "FileFunctions.h"
#include "Renderer.h"
#include "Camera.h"
namespace RuamEngine
{

	GLint Shader::maxTextureSlots= 0;
	unsigned int Shader::s_idInstanceCount = 0;


	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
		: m_vFilePath(GlobalizePath(vertexPath)), m_fFilePath(GlobalizePath(fragmentPath)), m_instanceId(s_idInstanceCount++), m_rendererId(0)
	{
		m_rendererId = CreateShader(vertexPath, fragmentPath);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
		SetUniformTextureSlots("u_albedoMap");
	}

	Shader::~Shader()
	{
		std::cout << "Shader Destroyed!\n";
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
	
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		// The second parameter indicates how many strings will be in the array of the source
		// The second one, if it's nullptr, indicates that the strings of the array will be null terminated
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		int compile_result;
		// glGetShaderiv returns a parameter from a shader object. In this case the parameter will be stored in result
		glGetShaderiv(id, GL_COMPILE_STATUS, &compile_result);
		if (compile_result == GL_FALSE)
		{
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(id, length, &length, message));
			std::cout << "Failed to compile: " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader! " << "\n";
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

		int link_result;
		glGetShaderiv(program, GL_LINK_STATUS, &link_result);
		if (link_result == GL_FALSE)
		{
			int length;
			GLCall(glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(program, length, &length, message));
			std::cout << "Failed to link: "<< "shader! " << "\n";
			std::cout << message << "\n";
			GLCall(glDeleteShader(program));
			return 0;
		}

		GLCall(glValidateProgram(program));

		int validation_result;
		glGetShaderiv(program, GL_VALIDATE_STATUS, &validation_result);
		if (validation_result == GL_FALSE)
		{
			int length;
			GLCall(glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(program, length, &length, message));
			std::cout << "Failed to validate: " << "shader! " << "\n";
			std::cout << message << "\n";
			GLCall(glDeleteShader(program));
			return 0;
		}

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}	

	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_rendererId));
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

	void Shader::SetUniformTextureSlots(const std::string& name)
	{
		std::vector<GLint> samplers = {};
		for (int i = 0; i < maxTextureSlots; i++)	
		{
			samplers.push_back(i);
		}
		Bind();
		auto loc = GetUniformLocation(name);
		GLCall(glUniform1iv(loc, maxTextureSlots, samplers.data()));
	}

	void Shader::LoadMaterial(const Material& material)
	{
		Bind();

		SetUniform4f("u_albedoColor", material.albedoColor.x, material.albedoColor.y, material.albedoColor.z, material.albedoColor.w);
		SetUniform1f("u_diffuse", material.m_diffuseIndex);
		SetUniform1f("u_specular", material.m_specularIndex);
	}	

	void Shader::UpdateCameraMatrices()
	{
		if (Camera::GetMainCamera() == nullptr)
		{
			//std::cout << "Not camera set!\n";
			SetUniformMat4f("u_view", glm::mat4(0.0f));
			SetUniformMat4f("u_projection", glm::mat4(0.0f));
			return;
		}
		Bind();
		SetUniformMat4f("u_view", Camera::GetMainCamera()->GetViewMatrix());
		SetUniformMat4f("u_projection", Camera::GetMainCamera()->GetProjectionMatrix());
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		Bind();

		int location;
		GLCall(location = glGetUniformLocation(m_rendererId, name.c_str()));
		if (location == -1)
			std::cout << "Warning: uniform " << name << " does not exist!\n";
	
		m_UniformLocationCache[name] = location;
	
		return location;
	}
}

