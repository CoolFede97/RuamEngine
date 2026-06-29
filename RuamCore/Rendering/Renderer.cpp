#include "Engine.h"
#include "Renderer.h"
#include "FrameBuffer.h"
#include "GLFW/glfw3.h"
#include "GlobalLight.h"
#include "RenderingConstants.h"
#include "RenderingCore.h"
#include "ResourceManager.h"
#include "SSBO.h"
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
	std::unordered_map<ShaderProgramName, std::unordered_map<ModelPath, MatricesSSBO>> Renderer::s_modelRUsMap = {};

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
            for (auto& [modelPath, matricesSSBO] : map) matricesSSBO->flush();
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
    void Renderer::Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        Skybox::Draw(viewMatrix, projectionMatrix);
        std::vector<ShaderProgramName> shaderProgramsToErase = {};
        for (auto& [shaderName, map] : s_modelRUsMap)
        {
            ShaderProgramSPtr shaderProgram = ResourceManager::GetShaderProgram(shaderName);
            if (shaderProgram) shaderProgram->updateCameraMatrices(viewMatrix, projectionMatrix);
            else
            {
                shaderProgramsToErase.push_back(shaderName);
                continue;
            }
            for (auto& [modelPath, matricesSSBO] : map)
            {
                ModelSPtr model = ResourceManager::GetModel(modelPath);
                if (matricesSSBO)
                {
                    matricesSSBO->submitData();
                    matricesSSBO->bindBufferBase(SSBOType::modelMatrices);
                }
                if (model)
                {
                    for (MeshSPtr mesh : model->m_meshes)
                    {
                        mesh->m_vertexArray->bind();
                        GlobalLight::LoadLightSettings(shaderProgram);
                        shaderProgram->loadMaterial(mesh->m_material.get());
                        mesh->submitData();
                        mesh->bindBuffersBase();
                        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->m_indices->currentSize()/sizeof(unsigned int), matricesSSBO->m_data.size()));
                    }
                }
            }
        }
        for (std::string shaderProgramName : shaderProgramsToErase) s_modelRUsMap.erase(shaderProgramName);
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
