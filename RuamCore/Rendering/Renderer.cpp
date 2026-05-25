#include "Engine.h"
#include "Renderer.h"
#include "FrameBuffer.h"
#include "GLFW/glfw3.h"
#include "GlobalLight.h"
#include "RenderUnit.h"
#include "RenderingConstants.h"
#include "RenderingCore.h"
#include "ResourceManager.h"
#include "SSBO.h"
#include "MeshRU.h"
#include "ShaderProgram.h"
#include "Skybox.h"
#include "Editor.h"

#include <cstddef>
#include <memory>
namespace RuamEngine
{
    RendererConfig Renderer::s_config;
    GLFWwindow* Renderer::s_window = nullptr;
    FrameBufferSPtr Renderer::s_editorFrameBuffer = nullptr;
    FrameBufferSPtr Renderer::s_gameFrameBuffer = nullptr;
    std::unordered_map<unsigned int, ShaderProgramSPtr> Renderer::s_shaderPrograms;
	std::unordered_map<GLuint, DrawingDataSPtr> Renderer::s_drawingDatas;
	using ShaderId = unsigned int; // instance id
	using ModelRUId = std::string; // model path
	using MatricesSSBO = SSBOSPtr<glm::mat4>;
	std::unordered_map<ShaderId, std::unordered_map<ModelRUId, MatricesSSBO>> Renderer::s_modelRUsMap = {};
	std::unordered_map<std::string, ModelRUWPtr> Renderer::s_modelRUs = {};

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
            s_editorFrameBuffer = std::make_shared<FrameBuffer>(s_config.windowWidth, s_config.windowHeight);
            s_gameFrameBuffer = std::make_shared<FrameBuffer>(s_config.windowWidth, s_config.windowHeight);

			DrawingDataSPtr basicDrawingData = CreateDrawingData(ShaderProgramType::general, "RuamCore/Rendering/Shaders/GeneralVertexShader.glsl", "RuamCore/Rendering/Shaders/GeneralFragmentShader.glsl");
			DrawingDataSPtr skyboxDrawingData = CreateDrawingData(ShaderProgramType::skybox, "RuamCore/Rendering/Shaders/SkyboxVertexShader.glsl", "RuamCore/Rendering/Shaders/SkyboxFragmentShader.glsl");

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
        for (auto& [shaderType, map] : s_modelRUsMap)
        {
            s_drawingDatas[shaderType]->m_program->bind();
            for (auto& [modelPath, matricesSSBO] : map) matricesSSBO->flush();
        }
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
		ShaderProgramSPtr newProgram = ResourceManager::CreateShaderProgram(vertexShaderPath, fragmentShaderPath);
        DrawingDataSPtr newDrawingData = std::make_unique<DrawingData>();
        s_drawingDatas[type] = newDrawingData;
        newDrawingData->m_program = newProgram;
        return newDrawingData;
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

	ModelRUSPtr Renderer::LoadModelRU(ModelSPtr model)
	{
        auto it = s_modelRUs.find(model->path());
        if (it != s_modelRUs.end())
        {
            if (!it->second.expired()) return it->second.lock();
        }
	    ModelRUSPtr newModelRU = std::make_shared<ModelRU>();
		newModelRU->setInstanceId(model->instanceId());
		for (MeshSPtr mesh : model->m_meshes)
		{
		    MeshRUSPtr newMeshRU = std::make_shared<MeshRU>(mesh->m_vertices, mesh->m_indices);
			newMeshRU->m_material = mesh->m_material;
			newModelRU->m_meshRUs.push_back(newMeshRU);
		}
		s_modelRUs[model->path()] = newModelRU;
		return newModelRU;
	}

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

    ModelRUSPtr Renderer::GetModelRU(const std::string& modelPath)
    {
        auto it = s_modelRUs.find(modelPath);
		if (it != s_modelRUs.end())
	    {
			if (!it->second.expired()) return it->second.lock();
	    }
		return nullptr;
    }

    void Renderer::Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        Skybox::Draw(viewMatrix, projectionMatrix);
        for (auto& [shaderType, map] : s_modelRUsMap)
        {
            auto program = s_drawingDatas[shaderType]->m_program;
            s_drawingDatas[shaderType]->m_program->updateCameraMatrices(viewMatrix, projectionMatrix);
            for (auto& [modelPath, matricesSSBO] : map)
            {
                ModelRUSPtr modelRU = GetModelRU(modelPath);
                if (matricesSSBO)
                {
                    matricesSSBO->submitData();
                    matricesSSBO->bindBufferBase(SSBOType::modelMatrices);
                }
                if (modelRU)
                {
                    for (MeshRUSPtr meshRU : modelRU->m_meshRUs)
                    {
                        meshRU->m_vertexArray->bind();
                        GlobalLight::LoadLightSettings(s_drawingDatas[shaderType]->m_program);
                        s_drawingDatas[shaderType]->m_program->loadMaterial(*GetShared(meshRU->m_material));
                        meshRU->submitData();
                        meshRU->bindBuffersBase();
                        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, meshRU->m_indices->currentSize()/sizeof(unsigned int), matricesSSBO->m_data.size()));
                    }
                }
            }
        }
   //      for (auto& [type,drawingData] : s_drawingDatas)
   //      {
			// drawingData->m_program->updateCameraMatrices(viewMatrix, projectionMatrix);

			// for (auto& [materialId, renderUnit] : drawingData->m_renderUnits)
   //          {
   //           	// The vertex array is useless and doesn't contain any information since I use SSBOs to pass the data into the shader.
   //           	// It's just there in order to satisfy OpenGL because it need to have one bound
   //         		renderUnit->m_vertexArray->bind();

   //              ShaderProgramSPtr program = drawingData->m_program;
   //              program->bind();
   //              GlobalLight::LoadLightSettings(program);
   //              program->loadMaterial(*renderUnit->m_material.lock());

   //              // Bind the SSBOs for this specific render unit
   //              renderUnit->submitData();
			// 	renderUnit->bindBuffersBase();
   //              GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, renderUnit->m_indices->currentSize()/sizeof(unsigned int), renderUnit->m_modelMatrices->m_data.size()));
   //          }
   //      }
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
