#include "Engine.h"
#include "Renderer.h"
#include "FrameBuffer.h"
#include "GLFW/glfw3.h"
#include "GlobalLight.h"
#include "RenderUnit.h"
#include "RenderingConstants.h"
#include "RenderingCore.h"
#include "ResourceManager.h"
#include "ShaderProgram.h"
#include "Skybox.h"
#include "Editor.h"

#include <cstddef>
#include <memory>
namespace RuamEngine
{
    RendererConfig Renderer::s_config;
    GLFWwindow* Renderer::s_window = nullptr;
    FrameBufferSPtr Renderer::s_frameBuffer = nullptr;
    std::unordered_map<unsigned int, ShaderProgramSPtr> Renderer::s_shaderPrograms;
	std::unordered_map<GLuint, DrawingDataSPtr> Renderer::s_drawingDatas;
	std::unordered_map<std::string, TextureSPtr> Renderer::s_texturesCache;
    std::unordered_map<GLenum, std::vector<TextureSPtr>> Renderer::s_texturesByType;
    std::unordered_map<GLenum, std::vector<unsigned int>> Renderer::s_textureFreeIndexesByType;
    std::unordered_map<GLenum, std::vector<GLuint64>> Renderer::s_handlesByType;
    std::unordered_map<GLenum, GLuint> Renderer::s_buffersByType;
    std::unordered_map<GLenum, int> Renderer::s_bindingsByType;

    // std::vector<glm::mat4> Renderer::matrices = {};

	bool Renderer::s_texturesUploaded = false;

    void Renderer::Init()
    {
        ASSERT(glfwInit());
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


        s_window = glfwCreateWindow(s_config.windowWidth, s_config.windowHeight, s_config.windowTitle, s_config.monitor, s_config.share);
        ASSERT(s_window);

        glfwMakeContextCurrent(s_window);

        glfwSetFramebufferSizeCallback(s_window, framebuffer_size_callback);

        glfwSwapInterval(1);

        ASSERT(glewInit() == GLEW_OK);

        if (s_config.depthTest)
        {
            GLCall(glEnable(GL_DEPTH_TEST));
            GLCall(glDepthFunc(GL_LEQUAL));
        }
        if (s_config.blend)
        {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        }


        {
            s_frameBuffer = std::make_shared<FrameBuffer>(s_config.windowWidth, s_config.windowHeight);

            s_bindingsByType[GL_TEXTURE_2D] = SSBOType::textures2D;
            s_bindingsByType[GL_TEXTURE_CUBE_MAP] = SSBOType::cubemaps;

            for (auto& [type, binding] : s_bindingsByType)
            {
                GLCall(glCreateBuffers(1, &s_buffersByType[type]));
            }
			DrawingDataSPtr basicDrawingData = CreateDrawingData(ShaderProgramType::general, "RuamCore/Rendering/Shaders/GeneralVertexShader.glsl", "RuamCore/Rendering/Shaders/GeneralFragmentShader.glsl");
			DrawingDataSPtr skyboxDrawingData = CreateDrawingData(ShaderProgramType::skybox, "RuamCore/Rendering/Shaders/SkyboxVertexShader.glsl", "RuamCore/Rendering/Shaders/SkyboxFragmentShader.glsl");

            AllocateTextureTypes();

            ResourceManager::Init();
            Skybox::Init();
        }

    }
    void Renderer::Shutdown()
    {
        glfwTerminate();
    }
    void Renderer::EndDraw()
    {
        glfwSwapBuffers(s_window);
    }
    void Renderer::BeginBatch()
    {
        Flush();
    }
    void Renderer::EndBatch()
    {
        for (auto& [type,drawingData] : s_drawingDatas)
        {
            drawingData->submitData();
        }
    }
    void Renderer::EndBatch(RenderUnit& renderUnit)
    {
        renderUnit.submitData();
    }
    void Renderer::Flush()
    {
        for (auto& [type,drawingData] : s_drawingDatas)
        {
            drawingData->flush();
        }
    }

    void Renderer::ClearRenderUnits()
    {
        for (auto& [type,drawingData] : s_drawingDatas)
        {
            drawingData->m_renderUnits.clear();
        }
    }

    void Renderer::ClearScreen()
    {
        if (s_config.useClearColor) GLCall(glClear(GL_COLOR_BUFFER_BIT));
        if (s_config.depthTest) GLCall(glClear(GL_DEPTH_BUFFER_BIT));
    }

    void Renderer::SetWindowSize(int width, int height)
    {
        s_config.windowWidth = width;
        s_config.windowHeight = height;
        glfwSetWindowSize(s_window, s_config.windowWidth, s_config.windowHeight);
    }

    void Renderer::SetWindowTitle(const char* title)
    {
        s_config.windowTitle = title;
        glfwSetWindowTitle(s_window, s_config.windowTitle);
    }

    void Renderer::SetClearColor(const glm::vec4& color)
    {
        s_config.clearColor = color;
        GLCall(glClearColor(s_config.clearColor.r, s_config.clearColor.g, s_config.clearColor.b, s_config.clearColor.a));
    }

    void Renderer::SetDepthTest(bool enable)
    {
        s_config.depthTest = enable;
        if (s_config.depthTest) GLCall(glEnable(GL_DEPTH_TEST));
        else GLCall(glDisable(GL_DEPTH_TEST));
    }

    void Renderer::SetBlend(bool enable, GLenum sfactor, GLenum dfactor)
    {
        s_config.blend = enable;
        s_config.blendSFactor = sfactor;
        s_config.blendDFactor = dfactor;
        if (s_config.blend)
        {
            GLCall(glEnable(GL_BLEND));
            GLCall(glBlendFunc(s_config.blendSFactor, s_config.blendDFactor));
        }
        else
        {
            GLCall(glDisable(GL_BLEND));
        }
    }


    // Creators -------------------------------------------------------------
    DrawingDataSPtr Renderer::CreateDrawingData(GLuint type, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
		ShaderProgramSPtr newProgram = Renderer::CreateProgram(vertexShaderPath, fragmentShaderPath);
        DrawingDataSPtr newDrawingData = std::make_unique<DrawingData>();
        s_drawingDatas[type] = newDrawingData;
        newDrawingData->m_program = newProgram;
        return newDrawingData;
	}

    ShaderProgramSPtr Renderer::CreateProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        ShaderProgramSPtr newProgram = std::make_shared<ShaderProgram>(vertexShaderPath, fragmentShaderPath);
        s_shaderPrograms[newProgram->instanceId()] = newProgram;
		return newProgram;
    }

    // If the render unit already exists, it returns the existing index. Otherwise, it creates a new render unit and returns its index.
    RenderUnitSPtr Renderer::CreateRenderUnit(ShaderProgramType shaderProgramType, MaterialWPtr material)
    {
        DrawingDataSPtr drawingData = s_drawingDatas[shaderProgramType];
		RenderUnitSPtr ru = GetRenderUnit(material, shaderProgramType);
        if (ru != nullptr)
        {
            return ru;
        }
        RenderUnitSPtr newRenderUnit = std::make_shared<RenderUnit>();
		newRenderUnit->m_material = material;
		newRenderUnit->m_drawingData = drawingData;
		newRenderUnit->m_program = drawingData->m_program;
        drawingData->m_renderUnits[material.lock()->id()] = newRenderUnit;
        return newRenderUnit;
	}

    // Registrations -------------------------------------------------------------

    // Shouldn't be called directly.
    // Returns the index of the handle in m_handlesByType[type].
    unsigned int Renderer::RegisterTexture(const TextureSPtr& texture)
    {
        GLenum type = texture->texType();

        if (s_handlesByType[type].size() >= maxTextureCountPerType)
        {
            std::cerr << "ERROR: Max texture limit of type " << type << " reached. The limit is " << maxTextureCountPerType << "\n";
            return 0;
        }

        GLuint64 newHandle;
        GLCall(newHandle = glGetTextureHandleARB(texture->glName()));
        ASSERT(newHandle != 0);

        GLCall(glMakeTextureHandleResidentARB(newHandle));

        unsigned int index;

        if (s_textureFreeIndexesByType[type].size()>0)
        {
        	index = s_textureFreeIndexesByType[type][0];
	        s_handlesByType[type][s_textureFreeIndexesByType[type][0]] = newHandle;
	        s_texturesByType[type][s_textureFreeIndexesByType[type][0]] = texture;
			s_textureFreeIndexesByType[type].erase(s_textureFreeIndexesByType[type].begin());
        }
        else
        {
		    s_handlesByType[type].push_back(newHandle);
		    s_texturesByType[type].push_back(texture);
			index = s_handlesByType[type].size()-1;
        }
        UpdateTextureType(type);
        return index;
    }

    void Renderer::UnregisterTexture(unsigned int textureIndex, GLenum type)
    {
        if (textureIndex >= s_texturesByType[type].size())
        {
            std::cout << "Warning: trying to unregister invalid texture of index " << textureIndex << "\n";
            return;
        }

        GLuint64 handle = s_handlesByType[type][textureIndex];
        GLCall(glMakeTextureHandleNonResidentARB(handle));

        s_texturesByType[type][textureIndex] = nullptr;
        s_handlesByType[type][textureIndex] = 0;
        s_textureFreeIndexesByType[type].push_back(textureIndex);

        UpdateTextureType(type);
    }

    // Destructions -------------------------------------------------------------

    // void Renderer::DestroyMaterial(unsigned int materialId)
    // {
    //     auto it = m_materials.find(materialId);
    //     if (it != m_materials.end())
    //     {
    //         m_materials.erase(it);
    //         std::cout << "Material with id " << materialId << " was destroyed successfully\n";
    //     }
    //     else std::cout << "Warning: Couldn't find Material of id " << materialId << ", and couldn't destroy it as a consequence\n";
    // }

    void Renderer::DestroyRenderUnit(RenderUnitSPtr renderUnit, DrawingDataSPtr drawingData)
    {
    	unsigned int materialId = renderUnit->m_material.lock()->id();
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
        auto it = s_shaderPrograms.find(programId);
        if (it != s_shaderPrograms.end())
        {
            s_shaderPrograms.erase(programId);
            std::cout << "Shader Program with id " << programId << " was destroyed successfully\n";
        }
        else std::cout << "Couldn't find shader program of id " << programId << " and therefore couldn't destroy it\n";
    }

    // Finders -------------------------------------------------------------

    // Finds if a render unit already exists in a certain drawing data, if not, returns nullptr
    RenderUnitSPtr Renderer::GetRenderUnit(MaterialWPtr material, ShaderProgramType shaderProgramType)
    {
        DrawingDataSPtr drawingData = s_drawingDatas[shaderProgramType];
        unsigned int materialId = material.lock()->id();
        auto units = drawingData->m_renderUnits;
        auto it = units.find(materialId);

        if (it != units.end()) return it->second;
        else return nullptr;
    }
    // Should be called before loading any textures (before any UpdateTextures/UpdateTextureType call)
    void Renderer::AllocateTextureTypes()
    {
        std::cout << "Buffer of textures 2D: " <<s_buffersByType[GL_TEXTURE_CUBE_MAP] << "\n";
        for (auto& [type, buffer] : s_buffersByType)
        {
            AllocateTextureType(type);
        }
        s_texturesUploaded = true;
    }

    void Renderer::AllocateTextureType(GLenum type)
    {
        ASSERT(s_handlesByType[type].size() < maxTextureCountPerType);
		ASSERT(!s_texturesUploaded);
        GLCall(glNamedBufferStorage(
            s_buffersByType[type],
            sizeof(GLuint64) * maxTextureCountPerType,
            NULL,
            GL_DYNAMIC_STORAGE_BIT
        ));
        GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, s_bindingsByType[type], s_buffersByType[type]));
    }

    // Should be used after calling UploadTextures()
    void Renderer::UpdateTextures()
    {
        for (auto& [type, handles] : s_handlesByType)
        {
            UpdateTextureType(type);
        }
	}

	void Renderer::UpdateTextureType(GLenum type)
	{
	    ASSERT(s_handlesByType[type].size() < maxTextureCountPerType);

        GLint size = 0;
        GLCall(glGetNamedBufferParameteriv(s_buffersByType[type], GL_BUFFER_SIZE, &size));

        GLCall(glNamedBufferSubData(
            s_buffersByType[type],
            0,
            sizeof(GLuint64) * s_handlesByType[type].size(),
            (const void*)s_handlesByType[type].data()
        ));
	}


    void Renderer::Draw()
    {
        for (auto& [type,drawingData] : s_drawingDatas)
        {
			drawingData->m_program->updateCameraMatrices(Editor::Camera());

			for (auto& [materialId, renderUnit] : drawingData->m_renderUnits)
            {
             	// The vertex array is useless and doesn't contain any information since I use SSBOs to pass the data into the shader.
             	// It's just there in order to satisfy OpenGL because it need to have one bound
           		renderUnit->m_vertexArray->bind();

                ShaderProgramSPtr program = drawingData->m_program;
                program->bind();
                GlobalLight::LoadLightSettings(program);
                program->loadMaterial(*renderUnit->m_material.lock());

                // Bind the SSBOs for this specific render unit
                renderUnit->submitData();
				renderUnit->bindBuffersBase();
                GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, renderUnit->m_indices->currentSize()/sizeof(unsigned int), renderUnit->m_modelMatricesBuffer->m_data.size()));
            }
        }
    }

    void Renderer::Draw(RenderUnit& renderUnit)
    {
    	// The vertex array is useless and doesn't contain any information since I use SSBOs to pass the data into the shader.
    	// It's just there in order to satisfy OpenGL because it need to have one bound
     	renderUnit.m_vertexArray->bind();

		ShaderProgramSPtr program = renderUnit.m_drawingData.lock()->m_program;
		program->bind();
		GlobalLight::LoadLightSettings(program);

		program->loadMaterial(*renderUnit.m_material.lock());

		// Bind the SSBOs for this specific render unit
		renderUnit.submitData();
		renderUnit.bindBuffersBase();

		GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, renderUnit.m_indices->currentSize()/sizeof(unsigned int), renderUnit.m_modelMatricesBuffer->m_data.size()));
    }

    void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        s_config.windowWidth = width;
        s_config.windowHeight = height;
        glViewport(0, 0, width, height);
        if (height==0) height = 1;
        float newAspectRatio = static_cast<float>(width) / static_cast<float>(height);
        Editor::Camera().setAspectRatio(newAspectRatio);
    }
}
