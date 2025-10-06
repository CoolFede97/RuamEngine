#pragma once

#include "RenderingCore.h"
#include "RenderingElements.h"
#include "RenderingConstants.h"

#include "DrawingData.h"
#include "RenderUnit.h"
#include "Material.h"

#include <unordered_map>

namespace RuamEngine
{
	class DrawingData;

    enum SSBOType
    {
        vertices = 0,
        indices = 1,
        modelMatrices = 2,
        textures = 3
    };

    // General data 
    struct RendererConfig
    {
        int windowWidth = 640;
        int windowHeight = 480;
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
        
        static void Init();
        static void Shutdown();
        static void EndDraw();
        static void BeginBatch();
        static void EndBatch();
		static void EndBatch(RenderUnit& renderUnit);
        static void ClearScreen();
        static void Flush();
        
		// Setters for RendererConfig
        static void SetWindowSize(int width, int height);
        static void SetWindowTitle(const char* title);
        static void SetClearColor(const glm::vec4& color);
        static void SetDepthTest(bool enable);
        static void SetBlend(bool enable, GLenum sfactor = GL_SRC_ALPHA, GLenum dfactor = GL_ONE_MINUS_SRC_ALPHA);
       

		// Getters for RendererConfig
        static int GetWindowWidth() { return m_config.windowWidth; }
        static int GetWindowHeight()  { return m_config.windowHeight; }
        static const char* GetWindowTitle()  { return m_config.windowTitle; }
        static glm::vec4 GetClearColor()  { return m_config.clearColor; }
        static bool GetUseClearColor()  { return m_config.useClearColor; }
        static bool GetDepthTest()  { return m_config.depthTest; }
        static bool GetBlend()  { return m_config.blend; }
        static GLFWmonitor* GetMonitor()  { return m_config.monitor; }
        static GLFWwindow* GetShare()  { return m_config.share; }
        static GLenum GetBlendSFactor()  { return m_config.blendSFactor; }
        static GLenum GetBlendDFactor()  { return m_config.blendDFactor; }


		static GLFWwindow* GetWindow() { return m_window; }


        static void Draw();
        static void Draw(RenderUnit& renderUnit);

        static std::unordered_map<Shader::PipelineType, std::unique_ptr<DrawingData>> m_drawingDataMap;
        static std::vector<TexturePtr> m_textures;
        static std::vector<uint32_t> m_textureHandles;
        //static std::vector<uvec2>
    private:
        static void CreateTexture(const std::string& texturePath);
        static void UploadTextures();
        static GLuint m_textureBuffer;
        static RendererConfig m_config;
        static GLFWwindow* m_window;
    };

}
