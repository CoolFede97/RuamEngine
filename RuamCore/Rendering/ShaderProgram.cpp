#include "ShaderProgram.h"
#include "FileFunctions.h"
#include "Renderer.h"
#include "EditorCamera.h"
namespace RuamEngine
{

	GLint ShaderProgram::maxTextureSlots= 0;
	unsigned int ShaderProgram::s_idInstanceCount = 0;


	ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)
		: m_vFilePath(GlobalizePath(vertexPath)), m_fFilePath(GlobalizePath(fragmentPath)), m_instanceId(s_idInstanceCount++), m_glName(0)
	{
		m_glName = createProgram(vertexPath, fragmentPath);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
	}

	ShaderProgram::~ShaderProgram()
	{
		std::cout << "Shader Destroyed!\n";
	}

	unsigned int ShaderProgram::compileShader(unsigned int type, const std::string& source)
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

	unsigned int ShaderProgram::createProgram(const std::string& vertexPath, const std::string& fragmentPath)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = compileShader(GL_VERTEX_SHADER, RelativeFileToString(vertexPath));
		unsigned int fs = compileShader(GL_FRAGMENT_SHADER, RelativeFileToString(fragmentPath));

		// A program is a group of glsl that can run on the GPU
		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));

		int link_result;
		glGetProgramiv(program, GL_LINK_STATUS, &link_result);
		if (link_result == GL_FALSE)
		{
			int length;
			GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetProgramInfoLog(program, length, &length, message));
			std::cout << "Failed to link: "<< "shader! " << "\n";
			std::cout << message << "\n";
			GLCall(glDeleteProgram(program));
			GLCall(glDeleteShader(vs));
			GLCall(glDeleteShader(fs));
			return 0;
		}

		GLCall(glValidateProgram(program));

		int validation_result;
		glGetProgramiv(program, GL_VALIDATE_STATUS, &validation_result);
		if (validation_result == GL_FALSE)
		{
			int length;
			GLCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetProgramInfoLog(program, length, &length, message));
			std::cout << "Failed to validate: " << "shader! " << "\n";
			std::cout << message << "\n";
			GLCall(glDeleteProgram(program));
			GLCall(glDeleteShader(vs));
			GLCall(glDeleteShader(fs));
			return 0;
		}

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}

	void ShaderProgram::bind() const
	{
		GLCall(glUseProgram(m_glName));
	}

	void ShaderProgram::unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void ShaderProgram::setUniform1i(const std::string& name, int value)
	{
		GLCall(glUniform1i(getUniformLocation(name), value));
	}

	void ShaderProgram::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
	}

	void ShaderProgram::setUniform3f(const std::string& name, float v0, float v1, float v2)
	{
		GLCall(glUniform3f(getUniformLocation(name), v0, v1, v2));
	}

	void ShaderProgram::setUniform1f(const std::string& name, float value)
	{
		GLCall(glUniform1f(getUniformLocation(name), value));
	}

	void ShaderProgram::setUniformMat4f(const std::string& name, glm::mat4 matrix)
	{
		GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
	}

	void ShaderProgram::setUniformTextureSlots(const std::string& name)
	{
		std::vector<GLint> samplers = {};
		for (int i = 0; i < maxTextureSlots; i++)
		{
			samplers.push_back(i);
		}
		bind();
		auto loc = getUniformLocation(name);
		GLCall(glUniform1iv(loc, maxTextureSlots, samplers.data()));
	}

	void ShaderProgram::loadMaterial(const Material& material)
	{
		bind();

		setUniform4f("u_baseColor", material.baseColor.x, material.baseColor.y, material.baseColor.z, material.baseColor.w);
		setUniform1f("u_diffuse", material.m_diffuseIndex);
		setUniform1f("u_specular", material.m_specularIndex);
		setUniform1f("u_shininess", material.m_shininess);
		setUniform1f("u_cubemap", material.m_cubemap);
	}

	void ShaderProgram::updateCameraMatrices()
	{
		bind();
		setUniformMat4f("u_view", EditorCamera::ViewMatrix());
		setUniformMat4f("u_projection", EditorCamera::ProjectionMatrix());
	}

	int ShaderProgram::getUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		bind();

		int location;
		GLCall(location = glGetUniformLocation(m_glName, name.c_str()));
		if (location == -1)
			std::cout << "Warning: uniform " << name << " does not exist!\n";

		m_UniformLocationCache[name] = location;

		return location;
	}
}
