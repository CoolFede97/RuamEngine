#include "Renderer.h"


namespace RuamEngine
{
    RendererConfig Renderer::m_config;
    GLFWwindow* Renderer::m_window = nullptr;
    std::unordered_map<unsigned int, std::unique_ptr<DrawingData>> Renderer::m_drawingDataMap;
    GLuint Renderer::m_textureBuffer = 0;
	std::vector<MaterialPtr> Renderer::m_materials;
    std::vector<TexturePtr> Renderer::m_textures;
    std::vector<GLuint64> Renderer::m_textureHandles;
	bool Renderer::texturesUploaded = false;
    void Renderer::Init()
    {
        ASSERT(glfwInit());
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


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
            GLCall(glCreateBuffers(1, &m_textureBuffer));
			CreateTexture("assets/sprites/defaultSprite.png");
            CreateTexture("assets/sprites/bigBrain.png");

			ShaderPtr genericShader = std::make_shared<Shader>("assets/shaders/GeneralVertexShader.glsl", "assets/shaders/GeneralFragmentShader.glsl");
            m_drawingDataMap.emplace(genericShader->GetInstanceID(), std::make_unique<DrawingData>());
            DrawingData& basicDrawingData = *m_drawingDataMap.at(genericShader->GetInstanceID());
            basicDrawingData.m_shader = genericShader;
			MaterialPtr genericMaterial = std::make_shared<Material>();
			m_materials.push_back(genericMaterial);
            basicDrawingData.m_renderUnits.emplace(genericMaterial->GetId(), RenderUnit(basicDrawingData.m_shader));
            RenderUnit& genericUnit = basicDrawingData.m_renderUnits.at(genericMaterial->GetId());
            basicDrawingData.m_shader->Bind();
            genericUnit.m_material = genericMaterial;
            VertexBufferLayout& genericLayout = *genericUnit.m_layout;
            //genericLayout.Reset();
            //genericLayout.Push<float>(3);
            //genericLayout.Push<float>(2);
            //genericLayout.Push<float>(1);
            //genericUnit.m_vertexArray->AddBuffer(*genericUnit.m_vertexBuffer, *genericUnit.m_layout);

            UploadTextures();
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
            pair.second->SubmitBatchData();
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
            pair.second->Flush();
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

    // Returns index in the vector of handles
    unsigned int Renderer::CreateTexture(const std::string& texturePath)
    {
        TexturePtr newTex = std::make_shared<Texture>(texturePath);
        GLuint64 newHandle; 
        GLCall(newHandle = glGetTextureHandleARB(newTex->GetId()));
        ASSERT(newHandle != 0);

        GLCall(glMakeTextureHandleResidentARB(newHandle));

        m_textureHandles.push_back(newHandle);
        m_textures.push_back(newTex);
        return m_textureHandles.size()-1;
    }

    // Should be called only once when finished all the textures
    void Renderer::UploadTextures()
    {
        ASSERT(m_textureHandles.size() < maxTextureCount);
		ASSERT(!texturesUploaded);
        GLCall(glNamedBufferStorage(
            m_textureBuffer,
            sizeof(GLuint64) * maxTextureCount,
            (const void*)m_textureHandles.data(),
            GL_DYNAMIC_STORAGE_BIT
        ));
        GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SSBOType::textures, m_textureBuffer));
		texturesUploaded = true;
    }

    // Should be used after calling UploadTextures()
    void Renderer::UpdateTextures()
    {
        ASSERT(m_textureHandles.size() < maxTextureCount);
        GLCall(glNamedBufferSubData(
            m_textureBuffer,
            0,
            sizeof(GLuint64) * m_textureHandles.size(),
            (const void*)m_textureHandles.data()
        ));
	}

    MaterialPtr Renderer::CreateMaterial()
    {
		MaterialPtr newMaterial = std::make_shared<Material>();
		m_materials.push_back(newMaterial);
		return newMaterial;
    }

    void Renderer::Draw()
    {
        for (auto& drawingData : m_drawingDataMap)
        {
            for (auto& renderUnit : drawingData.second->m_renderUnits)
            {
                drawingData.second->m_shader->Bind();
                drawingData.second->m_shader->LoadMaterial(*renderUnit.second.m_material);
				drawingData.second->m_shader->UpdateCameraMatrices();
                GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, renderUnit.second.m_indices->GetCurrentSize()/sizeof(unsigned int), renderUnit.second.m_modelMatricesBuffer->m_data.size()));
            }
        }
    }

    void Renderer::Draw(RenderUnit& renderUnit)
    {
        renderUnit.m_shader->Bind();
        renderUnit.m_shader->LoadMaterial(*renderUnit.m_material);
        renderUnit.m_shader->UpdateCameraMatrices();
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, renderUnit.m_indices->GetCurrentSize() / sizeof(unsigned int), renderUnit.m_modelMatricesBuffer->m_data.size()));
    }

}
    
