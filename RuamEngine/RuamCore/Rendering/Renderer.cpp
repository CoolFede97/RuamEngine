#include "Renderer.h"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << "\n" <<
            "in file: " << file << "\n" <<
            "in line: " << line << "\n" <<
            "in function: " << function << "\n";

        return false;
    }
    return true;    
}
namespace RuamEngine
{
    RendererConfig Renderer::m_config;
    RendererState Renderer::m_state;
	GLFWwindow* Renderer::m_window = nullptr;
    void Renderer::Init()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        ASSERT(glfwInit());
        
        m_window = glfwCreateWindow(m_config.windowWidth, m_config.windowHeight, m_config.windowTitle, m_config.monitor, m_config.share);
        ASSERT(m_window);

        glfwMakeContextCurrent(m_window);

        glfwSwapInterval(1);

        ASSERT(glewInit() == GLEW_OK);

        if (m_config.depthTest) GLCall(glEnable(GL_DEPTH_TEST));
        if (m_config.blend)
        {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        }


        {
			m_state.m_vertexArray = std::make_shared<VertexArray>();
			m_state.m_vertexBuffer = std::make_shared<VertexBuffer>(nullptr, maxVertexSize * maxVertexCount);
			m_state.m_layout = std::make_shared<VertexBufferLayout>();
			m_state.m_indexBuffer = std::make_shared<IndexBuffer>(nullptr, maxIndexCount);
            m_state.m_shader = std::make_shared<Shader>("assets/shaders/GeneralVertexShader.glsl", "assets/shaders/GeneralFragmentShader.glsl");
        }

    }
    void Renderer::Shutdown()
    {
        glfwTerminate();
    }
    void Renderer::BeginDraw()
    {
        Clear();

    }
    void Renderer::EndDraw()
    {
        glfwSwapBuffers(m_window);
    }
    void Renderer::Clear()
    {
        if (m_config.useClearColor) GLCall(glClear(GL_COLOR_BUFFER_BIT));
		if (m_config.depthTest) GLCall(glClear(GL_DEPTH_BUFFER_BIT));
    }

    void Renderer::SetWindowSize(int width, int height)
    {
		m_config.windowWidth = width;
		m_config.windowHeight = height;
        glfwSetWindowSize(m_window, m_config.windowWidth, m_config.windowHeight);
    }

    void Renderer::SetWindowTitle(const char* title)
    {
		m_config.windowTitle = title;
		glfwSetWindowTitle(m_window, m_config.windowTitle);
    }

    void Renderer::SetClearColor(const glm::vec4& color)
    {
        m_config.clearColor = color;
		GLCall(glClearColor(m_config.clearColor.r, m_config.clearColor.g, m_config.clearColor.b, m_config.clearColor.a));
    }

    void Renderer::SetDepthTest(bool enable)
    {
        m_config.depthTest = enable;
        if (m_config.depthTest) GLCall(glEnable(GL_DEPTH_TEST));
		else GLCall(glDisable(GL_DEPTH_TEST));
    }

    void Renderer::SetBlend(bool enable, GLenum sfactor, GLenum dfactor)
    {
		m_config.blend = enable;
		m_config.blendSFactor = sfactor;
		m_config.blendDFactor = dfactor;
        if (m_config.blend)
        {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(m_config.blendSFactor, m_config.blendDFactor));
        }
        else
        {
			GLCall(glDisable(GL_BLEND));
        }
    }

    void Renderer::Draw()
    {
        m_state.m_vertexArray->Bind();
        m_state.m_shader->Bind();
        m_state.m_indexBuffer->Bind();

        // PREGUNTAR CHONA
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    }
}
