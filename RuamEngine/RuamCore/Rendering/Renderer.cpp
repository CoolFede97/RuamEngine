#include "Renderer.h"


namespace RuamEngine
{
    RendererConfig Renderer::m_config;
    GLFWwindow* Renderer::m_window = nullptr;
    GLuint Renderer::m_textureBuffer = 0;
    std::vector<ShaderProgramPtr> Renderer::m_shaderPrograms;
	std::vector<DrawingDataPtr> Renderer::m_drawingDatas;
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

			DrawingDataPtr basicDrawingData = CreateDrawingData("assets/shaders/GeneralVertexShader.glsl", "assets/shaders/GeneralFragmentShader.glsl");
			MaterialPtr genericMaterial = CreateMaterial();
			RenderUnitPtr genericRenderUnit = CreateRenderUnit(basicDrawingData, genericMaterial);
            VertexBufferLayout& genericLayout = *genericRenderUnit->m_layout;
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
        for (DrawingDataPtr drawingData : m_drawingDatas)
        {
            drawingData->SubmitBatchData();
        }
    }
    void Renderer::EndBatch(RenderUnit& renderUnit)
    {
        renderUnit.SubmitBatchData();
    }
    void Renderer::Flush()
    {
        for (DrawingDataPtr drawingData : m_drawingDatas)
        {
            drawingData->Flush();
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

    RenderUnitPtr Renderer::CreateRenderUnit(DrawingDataPtr drawingData, MaterialPtr material)
    {
        RenderUnitPtr newRenderUnit = std::make_shared<RenderUnit>();
        for (unsigned int i = 0; i < drawingData->m_renderUnits.size(); i++)
        {
            if (drawingData->m_renderUnits[i]->m_material->GetId() == material->GetId())
            {
                std::cout << "Warning: RenderUnit with the same material already exists in this DrawingData.\n";
            }
		}
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
    unsigned int Renderer::CreateTexture(const std::string& relativeTexturePath)
    {
		unsigned int foundIndex = FindTexture(GlobalizePath(relativeTexturePath));
        if (foundIndex != -1)
        {
            return foundIndex;
		}
        TexturePtr newTex = std::make_shared<Texture>(relativeTexturePath);
        GLuint64 newHandle; 
        GLCall(newHandle = glGetTextureHandleARB(newTex->GetId()));
        ASSERT(newHandle != 0);

        GLCall(glMakeTextureHandleResidentARB(newHandle));

        m_textureHandles.push_back(newHandle);
        m_textures.push_back(newTex);
        return m_textureHandles.size()-1;
    }

	// Returns -1 if not found, otherwise it returns the index in the vector of textures
    unsigned int Renderer::FindTexture(const std::string& absoluteTexturePath)
    {
        for (unsigned int i = 0; i < m_textures.size(); i++)
        {
            if (m_textures[i]->m_filePath == absoluteTexturePath)
            {
                return i;
            }
        }
        return -1;
	}

	// Finds if a material already exists in the renderer
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
        
        GLint size = 0;
        GLCall(glGetNamedBufferParameteriv(m_textureBuffer, GL_BUFFER_SIZE, &size));

        GLCall(glNamedBufferSubData(
            m_textureBuffer,
            0,
            sizeof(GLuint64) * m_textureHandles.size(),
            (const void*)m_textureHandles.data()
        ));
	}


    void Renderer::Draw()
    {
        for (DrawingDataPtr drawingData : m_drawingDatas)
        {
			drawingData->m_program->UpdateCameraMatrices();
            for (RenderUnitPtr renderUnit : drawingData->m_renderUnits)
            {
                drawingData->m_program->Bind();
                drawingData->m_program->LoadMaterial(*renderUnit->m_material);
				//std::cout << "Vertices count: " << renderUnit->m_vertices->GetCurrentSize() / sizeof(Vertex) << "\n";
    //            std::cout << "Indices count: " << renderUnit->m_indices->GetCurrentSize() / sizeof(unsigned int) << "\n";

                GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, renderUnit->m_indices->GetCurrentSize()/sizeof(unsigned int), renderUnit->m_modelMatricesBuffer->m_data.size()));
            }
        }
    }

    void Renderer::Draw(RenderUnit& renderUnit)
    {
        renderUnit.m_program->Bind();
        renderUnit.m_program->LoadMaterial(*renderUnit.m_material);
        renderUnit.m_program->UpdateCameraMatrices();
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, renderUnit.m_indices->GetCurrentSize() / sizeof(unsigned int), renderUnit.m_modelMatricesBuffer->m_data.size()));
    }

}
    
