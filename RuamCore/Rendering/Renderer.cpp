#include "AABB.h"
#include "Engine.h"
#include "Renderer.h"
#include "FrameBuffer.h"
#include "GLFW/glfw3.h"
#include "GizmosManager.h"
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
#if defined(__linux__)
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
#endif
        ASSERT(glfwInit());
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        s_window = glfwCreateWindow(s_config.windowWidth, s_config.windowHeight, s_config.windowTitle, s_config.monitor, s_config.share);
        ASSERT(s_window);

        glfwMakeContextCurrent(s_window);

        glfwSetFramebufferSizeCallback(s_window, framebuffer_size_callback);

        glfwSwapInterval(1);

        glewExperimental = GL_TRUE;
        GLenum glewStatus = glewInit();
        if (
            glewStatus != GLEW_OK
            #if defined(__linux__)
                && glewStatus != GLEW_ERROR_NO_GLX_DISPLAY
            #endif
        )
        {
            std::cerr << "GLEW initialization failed: "
                      << static_cast<unsigned int>(glewStatus) << " ("
                      << reinterpret_cast<const char*>(glewGetErrorString(glewStatus)) << ")\n";
            ASSERT(false);
        }
        glGetError();

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
            int framebufferWidth = 0;
            int framebufferHeight = 0;
            glfwGetFramebufferSize(s_window, &framebufferWidth, &framebufferHeight);
            s_editorFrameBuffer = std::make_shared<FrameBuffer>(framebufferWidth, framebufferHeight);
            s_gameFrameBuffer = std::make_shared<FrameBuffer>(framebufferWidth, framebufferHeight);

            ResourceManager::Init();
            Skybox::Init();
            GizmosManager::Init();
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

        GizmosManager::s_colliderVertices->flush();
        GizmosManager::s_colliderIndices->flush();
        GizmosManager::s_indexCount = 0;
        GizmosManager::s_vertexBaseCount = 0;
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

    void Renderer::DrawSkybox(Camera& camera)
    {
        Skybox::s_shaderProgram->updateCameraMatrices(camera.viewMatrix(), camera.projectionMatrix());
        Skybox::s_vertexArray->bind();
        Skybox::s_verticesSSBO->bindBufferBase(SSBOType::vertices);
        Skybox::s_indicesSSBO->bindBufferBase(SSBOType::indices);
        GLCall(glActiveTexture(GL_TEXTURE3));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::s_cubemap->glName()));
		GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, Skybox::s_indicesSSBO->currentSize()/sizeof(unsigned int), 1));
    }

    void Renderer::DrawModels(Camera& camera)
    {
        std::vector<ShaderProgramName> shaderProgramsToErase = {};
        for (auto& [shaderName, map] : s_modelRUsMap)
        {
            ShaderProgramSPtr shaderProgram = ResourceManager::GetShaderProgram(shaderName);
            if (shaderProgram)
            {
                shaderProgram->updateCameraMatrices(camera.viewMatrix(), camera.projectionMatrix());
                GlobalLight::LoadLightSettings(shaderProgram);
            }
            else
            {
                shaderProgramsToErase.push_back(shaderName);
                continue;
            }
            for (auto& [modelPath, matricesSSBO] : map)
            {
                ModelSPtr model = ResourceManager::GetModel(modelPath);
                if (!model || !matricesSSBO) continue;
                std::vector<glm::mat4> seenInstances = {};
                for (auto& matrix : matricesSSBO->data())
                {
                    if (camera.canSeeBox(TransformAABB(model->aabb(), matrix)))
                    {
                        seenInstances.push_back(matrix);
                    }
                }
                if (seenInstances.empty()) continue;
                matricesSSBO->submitExternalData(seenInstances);
                matricesSSBO->bindBufferBase(SSBOType::modelMatrices);
                for (MeshSPtr mesh : model->m_meshes)
                {
                    DrawMesh(mesh, shaderProgram, seenInstances);
                }
            }
        }
        for (std::string shaderProgramName : shaderProgramsToErase) s_modelRUsMap.erase(shaderProgramName);
    }
    void Renderer::DrawMesh(MeshSPtr mesh, ShaderProgramSPtr shaderProgram, std::vector<glm::mat4>& seenInstances)
    {
        // This function is expected to be called from DrawModels, which will already have bound the corresponding shader program
        mesh->m_vertexArray->bind();
        shaderProgram->loadMaterial(mesh->m_material.get());
        mesh->submitData();
        mesh->bindBuffersBase();
        GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->m_indices->currentSize()/sizeof(unsigned int), seenInstances.size()));
    }

    void Renderer::DrawGizmos(Camera& camera)
    {
        if (GizmosManager::s_colliderVertices->data().size() == 0) return;
        ShaderProgramSPtr shaderProgram = GizmosManager::s_shaderProgram;
        shaderProgram->bind();
        shaderProgram->updateCameraMatrices(camera.viewMatrix(), camera.projectionMatrix());
        GizmosManager::s_vertexArray->bind();

        GizmosManager::s_colliderVertices->submitData();
        GizmosManager::s_colliderVertices->bindBufferBase(SSBOType::vertices);
        GizmosManager::s_colliderIndices->submitData();
        GizmosManager::s_colliderIndices->bindBufferBase(SSBOType::indices);

        GLCall(glDrawArrays(GL_LINES, 0, GizmosManager::s_indexCount));
    }

    void Renderer::Draw(Camera& camera)
    {
        DrawSkybox(camera);
        DrawModels(camera);
        DrawGizmos(camera);
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
