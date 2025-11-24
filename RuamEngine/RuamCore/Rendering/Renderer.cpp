#include "Renderer.h"


namespace RuamEngine
{
    RendererConfig Renderer::m_config;
    GLFWwindow* Renderer::m_window = nullptr;
    GLuint Renderer::m_texture2DBuffer = 0;
    std::vector<ShaderProgramPtr> Renderer::m_shaderPrograms;
	std::vector<DrawingDataPtr> Renderer::m_drawingDatas;
	std::vector<MaterialPtr> Renderer::m_materials;
    std::vector<Texture2DPtr> Renderer::m_textures2D;
    std::vector<GLuint64> Renderer::m_texture2DHandles;

    std::vector<glm::mat4> Renderer::matrices = {};

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

        glfwSwapInterval(0);

        ASSERT(glewInit() == GLEW_OK);

        if (m_config.depthTest) GLCall(glEnable(GL_DEPTH_TEST));
        if (m_config.blend)
        {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        }


        {
            GLCall(glCreateBuffers(1, &m_texture2DBuffer));
			CreateTexture2D("assets/sprites/defaultSprite.png");

			DrawingDataPtr basicDrawingData = CreateDrawingData("assets/shaders/GeneralVertexShader.glsl", "assets/shaders/GeneralFragmentShader.glsl");
			/*MaterialPtr genericMaterial = CreateMaterial();
			RenderUnitPtr genericRenderUnit = CreateRenderUnit(basicDrawingData, genericMaterial);
            VertexBufferLayout& genericLayout = *genericRenderUnit->m_layout;*/
            //genericLayout.Reset();
            //genericLayout.Push<float>(3);
            //genericLayout.Push<float>(2);
            //genericLayout.Push<float>(1);
            //genericUnit.m_vertexArray->AddBuffer(*genericUnit.m_vertexBuffer, *genericUnit.m_layout);

            UploadTextures2D();
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
        for (DrawingDataPtr drawingData : m_drawingDatas)
        {
            drawingData->SubmitData();
        }
    }
    void Renderer::EndBatch(RenderUnit& renderUnit)
    {
        renderUnit.SubmitData();
    }
    void Renderer::Flush()
    {
        for (DrawingDataPtr drawingData : m_drawingDatas)
        {
            drawingData->Flush();
        }
    }

    void Renderer::ClearRenderUnits()
    {
        for (DrawingDataPtr drawingData : m_drawingDatas)
        {
            drawingData->m_renderUnits.clear();
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

    DrawingDataPtr Renderer::CreateDrawingData(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
		ShaderProgramPtr newProgram = Renderer::CreateProgram(vertexShaderPath, fragmentShaderPath);
        m_shaderPrograms.push_back(newProgram);
        DrawingDataPtr newDrawingData = std::make_unique<DrawingData>();
        m_drawingDatas.push_back(newDrawingData);
        newDrawingData->m_program = newProgram;
        return newDrawingData;
	}

    ShaderProgramPtr Renderer::CreateProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        ShaderProgramPtr newProgram = std::make_shared<ShaderProgram>(vertexShaderPath, fragmentShaderPath);
        m_shaderPrograms.push_back(newProgram);
		return newProgram;
    }

    // If the render unit already exists, it returns the existing index. Otherwise, it creates a new render unit and returns its index.
    RenderUnitPtr Renderer::CreateRenderUnit(DrawingDataPtr drawingData, MaterialPtr material)
    {
		unsigned int foundIndex = FindRenderUnit(material, drawingData);
        if (foundIndex != -1)
        {
            return drawingData->m_renderUnits[foundIndex];
        }
        RenderUnitPtr newRenderUnit = std::make_shared<RenderUnit>();
		newRenderUnit->m_material = material;
		newRenderUnit->m_drawingData = drawingData;
		newRenderUnit->m_program = drawingData->m_program;
        drawingData->m_renderUnits.push_back(newRenderUnit);
        return newRenderUnit;
	}

    MaterialPtr Renderer::CreateMaterial()
    {
		MaterialPtr newMaterial = std::make_shared<Material>();
		m_materials.push_back(newMaterial);
		return newMaterial;
    }

    // If the texture already exists, it returns the existing index. Otherwise, it creates a new texture and returns its index.
    unsigned int Renderer::CreateTexture2D(const std::string& relativeTexturePath)
    {
		unsigned int foundIndex = FindTexture2D(GlobalizePath(relativeTexturePath));
        if (foundIndex != -1)
        {
            return foundIndex;
		}
        Texture2DPtr newTex = std::make_shared<Texture2D>(relativeTexturePath);
        GLuint64 newHandle;
        GLCall(newHandle = glGetTextureHandleARB(newTex->GetId()));
        ASSERT(newHandle != 0);

        GLCall(glMakeTextureHandleResidentARB(newHandle));

        m_texture2DHandles.push_back(newHandle);
        m_textures2D.push_back(newTex);
        return m_texture2DHandles.size()-1;
    }

	// Returns -1 if not found, otherwise it returns the index in the vector of Texture2Ds
    unsigned int Renderer::FindTexture2D(const std::string& absoluteTexturePath)
    {
        for (unsigned int i = 0; i < m_textures2D.size(); i++)
        {
            if (m_textures2D[i]->m_filePath == absoluteTexturePath)
            {
                return i;
            }
        }
        return -1;
	}

	// Finds if a material already exists in the renderer, if not, returns -1
    unsigned int Renderer::FindMaterial(MaterialPtr material)
    {
        for (unsigned int i = 0; i < m_materials.size(); i++)
        {
            if (m_materials[i]->GetId() == material->GetId())
            {
                return i;
            }
        }
        return -1;
	}

    // Finds if a render unit already exists in a certain drawing data, if not, returns -1
    unsigned int Renderer::FindRenderUnit(MaterialPtr material, DrawingDataPtr drawingData)
    {
        for (unsigned int i = 0; i < drawingData->m_renderUnits.size(); i++)
        {
            if (drawingData->m_renderUnits[i]->m_material->GetId() == material->GetId())
            {
                return i;
            }
        }
        return -1;
    }
    // Should be called only once when finished all the textures
    void Renderer::UploadTextures2D()
    {
        ASSERT(m_texture2DHandles.size() < maxTextureCount);
		ASSERT(!texturesUploaded);

        GLCall(glNamedBufferStorage(
            m_texture2DBuffer,
            sizeof(GLuint64) * maxTextureCount,
            (const void*)m_texture2DHandles.data(),
            GL_DYNAMIC_STORAGE_BIT
        ));
        GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SSBOType::textures, m_texture2DBuffer));
		texturesUploaded = true;
    }

    // Should be used after calling UploadTextures()
    void Renderer::UpdateTextures2D()
    {
        ASSERT(m_texture2DHandles.size() < maxTextureCount);

        GLint size = 0;
        GLCall(glGetNamedBufferParameteriv(m_texture2DBuffer, GL_BUFFER_SIZE, &size));

        GLCall(glNamedBufferSubData(
            m_texture2DBuffer,
            0,
            sizeof(GLuint64) * m_texture2DHandles.size(),
            (const void*)m_texture2DHandles.data()
        ));
	}


    void Renderer::Draw()
    {
        for (DrawingDataPtr drawingData : m_drawingDatas)
        {
            // std::cout << "Render units count: " << drawingData->m_renderUnits.size() << "\n";
			drawingData->m_program->UpdateCameraMatrices();
            for (RenderUnitPtr renderUnit : drawingData->m_renderUnits)
            {
                drawingData->m_program->Bind();
                drawingData->m_program->LoadMaterial(*renderUnit->m_material);

                // Bind the SSBOs for this specific render unit
                renderUnit->SubmitData();
				renderUnit->BindBuffersBase();

                GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, renderUnit->m_indices->GetCurrentSize()/sizeof(unsigned int), renderUnit->m_modelMatricesBuffer->m_data.size()));
            }
        }
    }

    void Renderer::Draw(RenderUnit& renderUnit)
    {
        renderUnit.m_program->Bind();
        renderUnit.m_program->LoadMaterial(*renderUnit.m_material);
        renderUnit.m_program->UpdateCameraMatrices();
        renderUnit.SubmitData();
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, renderUnit.m_indices->GetCurrentSize() / sizeof(unsigned int), renderUnit.m_modelMatricesBuffer->m_data.size()));
    }

}
