#pragma once

#include "Model.h"
#include "RenderingCore.h"
#include "RenderingElements.h"

#include "DrawingData.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "FrameBuffer.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace RuamEngine
{
	class DrawingData;
	class ResourceManager;

    // General data
    struct RendererConfig
    {
        int windowWidth = 853;
        int windowHeight = 640;
        const char* windowTitle = "Cool Window";


        glm::vec4 clearColor = glm::vec4(0.24f, 0.24f, 0.16f, 1.0f);
        bool useClearColor = true;

        bool depthTest = true;
        bool blend = true;


        GLFWmonitor* monitor = NULL;
        GLFWwindow* share = NULL;

		GLenum blendSFactor = GL_SRC_ALPHA;
		GLenum blendDFactor = GL_ONE_MINUS_SRC_ALPHA;

    };

    class Renderer
    {
    public:
		// Setters for RendererConfig
        static void SetWindowSize(int width, int height);
        static void SetWindowTitle(const char* title);
        static void SetClearColor(const glm::vec4& color);
        static void SetDepthTest(bool enable);
        static void SetBlend(bool enable, GLenum sfactor = GL_SRC_ALPHA, GLenum dfactor = GL_ONE_MINUS_SRC_ALPHA);

		// Getters for RendererConfig
        static int GetWindowWidth() { return s_config.windowWidth; }
        static int GetWindowHeight()  { return s_config.windowHeight; }
        static const char* GetWindowTitle()  { return s_config.windowTitle; }
        static glm::vec4 GetClearColor()  { return s_config.clearColor; }
        static bool GetUseClearColor()  { return s_config.useClearColor; }
        static bool GetDepthTest()  { return s_config.depthTest; }
        static bool GetBlend()  { return s_config.blend; }
        static GLFWmonitor* GetMonitor()  { return s_config.monitor; }
        static GLFWwindow* GetShare()  { return s_config.share; }
        static GLenum GetBlendSFactor()  { return s_config.blendSFactor; }
        static GLenum GetBlendDFactor()  { return s_config.blendDFactor; }
		static GLFWwindow* GetWindow() { return s_window; }
		static int WindowShouldClose() { return glfwWindowShouldClose(s_window); }

    private:
        static void Init();
        static void Shutdown();

        static void EndDraw();
        static void ClearScreen();

        // Destroyers
        static void DestroyShaderProgram(unsigned int programId);

        static void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    public:
		static std::unordered_map<ShaderProgramName, std::unordered_map<ModelPath, MatricesSSBO>> s_modelRUsMap;
    private:
        static RendererConfig s_config;
        static GLFWwindow* s_window;

        static FrameBufferSPtr s_editorFrameBuffer;
        static FrameBufferSPtr s_gameFrameBuffer;

        friend class ResourceManager;
        friend class Engine;
        friend class Editor;
    };

}
