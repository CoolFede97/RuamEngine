#include "Renderer.h"
#include "FileFunctions.h"
#include "GlobalLight.h"
#include "RenderUnit.h"
#include "RenderingConstants.h"
#include "RenderingCore.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>

namespace RuamEngine
{
    RendererConfig Renderer::m_config;
    GLFWwindow* Renderer::m_window = nullptr;
    std::unordered_map<unsigned int, ShaderProgramPtr> Renderer::m_shaderPrograms;
	std::unordered_map<GLuint, DrawingDataPtr> Renderer::m_drawingDatas;
	std::unordered_map<unsigned int, MaterialPtr> Renderer::m_materials;
	std::unordered_map<std::string, TexturePtr> Renderer::m_texturesCache;
    std::unordered_map<GLenum, std::vector<TexturePtr>> Renderer::m_texturesByType;
    std::unordered_map<GLenum, std::vector<GLuint64>> Renderer::m_handlesByType;
    std::unordered_map<GLenum, GLuint> Renderer::m_buffersByType;
    std::unordered_map<GLenum, int> Renderer::m_bindingsByType;

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

        if (m_config.depthTest)
        {
            GLCall(glEnable(GL_DEPTH_TEST));
            GLCall(glDepthFunc(GL_LEQUAL));
        }
        if (m_config.blend)
        {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        }


        {
            m_bindingsByType[GL_TEXTURE_2D] = SSBOType::textures2D;
            m_bindingsByType[GL_TEXTURE_CUBE_MAP] = SSBOType::cubemaps;

            for (auto& [type, binding] : m_bindingsByType)
            {
                GLCall(glCreateBuffers(1, &m_buffersByType[type]));
            }
			DrawingDataPtr basicDrawingData = CreateDrawingData(ShaderProgramType::general, "RuamCore/Rendering/Shaders/GeneralVertexShader.glsl", "RuamCore/Rendering/Shaders/GeneralFragmentShader.glsl");
			DrawingDataPtr skyboxDrawingData = CreateDrawingData(ShaderProgramType::skybox, "RuamCore/Rendering/Shaders/SkyboxVertexShader.glsl", "RuamCore/Rendering/Shaders/SkyboxFragmentShader.glsl");

            AllocateTextureTypes();

            ResourceManager::Init();
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
        for (auto& [type,drawingData] : m_drawingDatas)
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
        for (auto& [type,drawingData] : m_drawingDatas)
        {
            drawingData->Flush();
        }
    }

    void Renderer::ClearRenderUnits()
    {
        for (auto& [type,drawingData] : m_drawingDatas)
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


    // Creators -------------------------------------------------------------
    DrawingDataPtr Renderer::CreateDrawingData(GLuint type, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
		ShaderProgramPtr newProgram = Renderer::CreateProgram(vertexShaderPath, fragmentShaderPath);
        DrawingDataPtr newDrawingData = std::make_unique<DrawingData>();
        m_drawingDatas[type] = newDrawingData;
        newDrawingData->m_program = newProgram;
        return newDrawingData;
	}

    ShaderProgramPtr Renderer::CreateProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        ShaderProgramPtr newProgram = std::make_shared<ShaderProgram>(vertexShaderPath, fragmentShaderPath);
        m_shaderPrograms[newProgram->GetInstanceId()] = newProgram;
		return newProgram;
    }

    // If the render unit already exists, it returns the existing index. Otherwise, it creates a new render unit and returns its index.
    RenderUnitPtr Renderer::CreateRenderUnit(DrawingDataPtr drawingData, MaterialPtr material)
    {
		RenderUnitPtr ru = GetRenderUnit(material, drawingData);
        if (ru != nullptr)
        {
            return ru;
        }
        RenderUnitPtr newRenderUnit = std::make_shared<RenderUnit>();
		newRenderUnit->m_material = material;
		newRenderUnit->m_drawingData = drawingData;
		newRenderUnit->m_program = drawingData->m_program;
        drawingData->m_renderUnits[material->GetId()] = newRenderUnit;
        return newRenderUnit;
	}

    MaterialPtr Renderer::CreateMaterial()
    {
		MaterialPtr newMaterial = std::make_shared<Material>();
		m_materials[newMaterial->GetId()] = newMaterial;
		return newMaterial;
    }

    // Registrations -------------------------------------------------------------

    // If the texture already exists, it returns the existing index. Otherwise, it creates a new texture and returns its index.
    unsigned int Renderer::RegisterTexture(TexturePtr texture)
    {
        GLenum type = texture->GetType();

        if (m_handlesByType[type].size() >= maxTextureCountPerType)
        {
            std::cerr << "ERROR: Max texture limit of type " << type << " reached. The limit is " << maxTextureCountPerType << "\n";
            return 0;
        }

        for (unsigned int i = 0; i < m_texturesByType[type].size(); i++)
        {
            if (texture->GetPath() == m_texturesByType[type][i]->GetPath()) return i;
        }

        GLuint64 newHandle;
        GLCall(newHandle = glGetTextureHandleARB(texture->GetId()));
        ASSERT(newHandle != 0);

        GLCall(glMakeTextureHandleResidentARB(newHandle));

        m_handlesByType[type].push_back(newHandle);
        m_texturesByType[type].push_back(texture);
        UpdateTextureType(type);
        return m_handlesByType[type].size()-1;
    }

    void Renderer::UnregisterTexture(unsigned int textureIndex, GLenum type)
    {
        if (textureIndex >= m_texturesByType[type].size())
        {
            std::cout << "Warning: trying to unregister invalid texture of index " << textureIndex << "\n";
            return;
        }

        GLuint64 handle = m_handlesByType[type][textureIndex];
        GLCall(glMakeTextureHandleNonResidentARB(handle));

        m_texturesByType[type][textureIndex] = nullptr;
        m_handlesByType[type][textureIndex] = 0;

        UpdateTextureType(type);
    }

    // Destructions -------------------------------------------------------------

    void Renderer::DestroyMaterial(unsigned int materialId)
    {
        auto it = m_materials.find(materialId);
        if (it != m_materials.end())
        {
            m_materials.erase(it);
            std::cout << "Material with id " << materialId << " was destroyed successfully\n";
        }
        else std::cout << "Warning: Couldn't find Material of id " << materialId << ", and couldn't destroy it as a consequence\n";
    }

    void Renderer::DestroyRenderUnit(RenderUnitPtr renderUnit, DrawingDataPtr drawingData)
    {
    	unsigned int materialId = renderUnit->m_material->GetId();
        auto& units = drawingData->m_renderUnits;
        auto it = units.find(materialId);

        if (it != units.end())
        {
            units.erase(it);
            std::cout << "A Render Unit was destroyed successfully\n";
        }
        else std::cout << "Warning: Couldn't find render unit and therefore couldn't destroy it\n";
    }

    void Renderer::DestroyShaderProgram(unsigned int programId)
    {
        auto it = m_shaderPrograms.find(programId);
        if (it != m_shaderPrograms.end())
        {
            m_shaderPrograms.erase(programId);
            std::cout << "Shader Program with id " << programId << " was destroyed successfully\n";
        }
        else std::cout << "Couldn't find shader program of id " << programId << " and therefore couldn't destroy it\n";
    }

    // Finders -------------------------------------------------------------

	// Finds if a material already exists in the renderer, if not, returns -1
    unsigned int Renderer::FindMaterial(MaterialPtr material)
    {
        auto it = m_materials.find(material->GetId());
        if (it != m_materials.end()) return it->first;
        return -1;
	}

    // Finds if a render unit already exists in a certain drawing data, if not, returns nullptr
    RenderUnitPtr Renderer::GetRenderUnit(MaterialPtr material, DrawingDataPtr drawingData)
    {
    	unsigned int materialId = material->GetId();
     	auto units = drawingData->m_renderUnits;
    	auto it = units.find(materialId);
     	if (it != units.end()) return it->second;
      else return nullptr;
    }
    // Should be called before loading any textures (before any UpdateTextures/UpdateTextureType call)
    void Renderer::AllocateTextureTypes()
    {
        std::cout << "Buffer of textures 2D: " <<m_buffersByType[GL_TEXTURE_CUBE_MAP] << "\n";
        for (auto& [type, buffer] : m_buffersByType)
        {
            AllocateTextureType(type);
        }
        texturesUploaded = true;
    }

    void Renderer::AllocateTextureType(GLenum type)
    {
        ASSERT(m_handlesByType[type].size() < maxTextureCountPerType);
		ASSERT(!texturesUploaded);
        GLCall(glNamedBufferStorage(
            m_buffersByType[type],
            sizeof(GLuint64) * maxTextureCountPerType,
            NULL,
            GL_DYNAMIC_STORAGE_BIT
        ));
        GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindingsByType[type], m_buffersByType[type]));
    }

    // Should be used after calling UploadTextures()
    void Renderer::UpdateTextures()
    {
        for (auto& [type, handles] : m_handlesByType)
        {
            UpdateTextureType(type);
        }
	}

	void Renderer::UpdateTextureType(GLenum type)
	{
	    ASSERT(m_handlesByType[type].size() < maxTextureCountPerType);

        GLint size = 0;
        GLCall(glGetNamedBufferParameteriv(m_buffersByType[type], GL_BUFFER_SIZE, &size));

        GLCall(glNamedBufferSubData(
            m_buffersByType[type],
            0,
            sizeof(GLuint64) * m_handlesByType[type].size(),
            (const void*)m_handlesByType[type].data()
        ));
	}


    void Renderer::Draw()
    {
        for (auto& [type,drawingData] : m_drawingDatas)
        {
            // std::cout << "Render units count: " << drawingData->m_renderUnits.size() << "\n";
			drawingData->m_program->UpdateCameraMatrices();
            for (auto& [materialId, renderUnit] : drawingData->m_renderUnits)
            {
            	// The vertex array is useless and doesn't contain any information since I use SSBOs to pass the data into the shader.
             	// It's just there in order to satisfy OpenGL because it need to have one bound
           		renderUnit->m_vertexArray->Bind();

                ShaderProgramPtr program = drawingData->m_program;
                program->Bind();
                GlobalLight::LoadLightSettings(program);

                program->LoadMaterial(*renderUnit->m_material);

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

        // Is something doesn't work when this line functions is called due to an exceeded amount of data in a batch (which is okay to happen, may be the solution is to uncomment the line below)
        // renderUnit.SubmitData();
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, renderUnit.m_indices->GetCurrentSize() / sizeof(unsigned int), renderUnit.m_modelMatricesBuffer->m_data.size()));
    }

}
