#include "Renderer.h"


namespace RuamEngine
{
    RendererConfig Renderer::m_config;
	GLFWwindow* Renderer::m_window = nullptr;
    std::unordered_map<Shader::PipelineType, DrawingData> Renderer::m_drawingDataMap;
    void Renderer::Init()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        ASSERT(glfwInit());
        
        m_window = glfwCreateWindow(m_config.windowWidth, m_config.windowHeight, m_config.windowTitle, m_config.monitor, m_config.share);
        ASSERT(m_window);

        glfwMakeContextCurrent(m_window);

        glfwSwapInterval(0);

        ASSERT(glewInit() == GLEW_OK);

        if (m_config.depthTest) GLCall(glEnable(GL_DEPTH_TEST));
        if (m_config.blend)
        {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        }


        {
			m_drawingDataMap.emplace(Shader::PipelineType::Generic, DrawingData(Shader::PipelineType::Generic));
			DrawingData& basicDrawingData = m_drawingDataMap.at(Shader::PipelineType::Generic);
            basicDrawingData.m_shader = std::make_shared<Shader>("assets/shaders/GeneralVertexShader.glsl", "assets/shaders/GeneralFragmentShader.glsl");
			basicDrawingData.m_renderUnits.emplace(Material::MaterialType::Generic, RenderUnit(basicDrawingData.m_shader));
            RenderUnit& genericUnit = basicDrawingData.m_renderUnits.at(Material::MaterialType::Generic);
            basicDrawingData.m_shader->Bind();
			genericUnit.m_material = std::make_shared<Material>(Material::MaterialType::Generic);
			genericUnit.m_material->albedoColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
            Texture trash("assets/sprites/albedoColor.png");
            genericUnit.m_material->textures.push_back(Texture("assets/sprites/albedoColor.png"));
            genericUnit.m_shader->SetUniform1i("u_albedoMap", 0);
            VertexBufferLayout& genericLayout = *genericUnit.m_layout;
            genericLayout.Reset();
            genericLayout.Push<float>(3);
            genericLayout.Push<float>(2);
            genericLayout.Push<float>(1);
            genericUnit.m_vertexArray->AddBuffer(*genericUnit.m_vertexBuffer, *genericUnit.m_layout);
        }

    }
    void Renderer::Shutdown()
    {
        glfwTerminate();
    }
    void Renderer::EndDraw()
    {
        glfwSwapBuffers(m_window);
    }
    void Renderer::BeginBatch()
    {
        Flush();
    }
    void Renderer::EndBatch()
    {
        for (auto& pair : m_drawingDataMap)
        {
            pair.second.SubmitBatchData();
        }
    }
    void Renderer::EndBatch(RenderUnit& renderUnit)
    {
        renderUnit.SubmitBatchData();
    }
    void Renderer::Flush()
    {
        for (auto& pair : m_drawingDataMap)
        {
            pair.second.Flush();
        }
    }

    void Renderer::ClearScreen()
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
        for (auto& drawingData : m_drawingDataMap)
        {
            for (auto& renderUnit : drawingData.second.m_renderUnits)
            {
                drawingData.second.m_shader->Bind();
                Texture texture = Texture("assets/sprites/albedoColor.png");
                drawingData.second.m_shader->LoadMaterial(*renderUnit.second.m_material, texture);
                renderUnit.second.m_vertexArray->Bind();
                renderUnit.second.m_indexBuffer->Bind();
                GLCall(glDrawElements(GL_TRIANGLES, renderUnit.second.m_indexBuffer->GetIndexCount(), GL_UNSIGNED_INT, nullptr));
            }
        }
    }

    void Renderer::Draw(RenderUnit& renderUnit)
    {
        renderUnit.m_vertexArray->Bind();
        renderUnit.m_shader->Bind();
        /*Texture texture = Texture("assets/sprites/albedoColor.png");
        Texture textureB = Texture("assets/sprites/albedoColor.png");
        ASSERT(*texture.m_LocalBuffer == *textureB.m_LocalBuffer);
		*/
        Texture texture = Texture("assets/sprites/albedoColor.png");
        renderUnit.m_shader->LoadMaterial(*renderUnit.m_material, texture);
        renderUnit.m_indexBuffer->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, renderUnit.m_indexBuffer->GetIndexCount(), GL_UNSIGNED_INT, nullptr));   
    }

    void Renderer::DrawQuads()
    {
        
    }
}
