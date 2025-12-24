#pragma once

#include "RenderingCore.h"
#include "RenderingElements.h"
#include "RenderingConstants.h"

#include "DrawingData.h"
#include "RenderUnit.h"
#include "Material.h"
#include "Texture.h"
#include "Texture2D.h"
#include "Cubemap.h"
#include "Model.h"
#include <unordered_map>
#include <cstdint>
#include <vector>

namespace RuamEngine
{
	class DrawingData;

    enum SSBOType
    {
        vertices = 0,
        indices = 1,
        modelMatrices = 2,
        textures2D = 3,
        cubemaps = 4
    };

    enum ShaderProgramType
    {
        general = 0,
        skybox = 1
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
        static void ClearRenderUnits();

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
		static int WindowShouldClose() { return glfwWindowShouldClose(m_window); }

		// Creators
        static DrawingDataPtr CreateDrawingData(GLuint type, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		static ShaderProgramPtr CreateProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        static RenderUnitPtr CreateRenderUnit(DrawingDataPtr drawingData, MaterialPtr material);
		static MaterialPtr CreateMaterial();

		// Finders
        static unsigned int FindMaterial(MaterialPtr material);
        static RenderUnitPtr GetRenderUnit(MaterialPtr material, DrawingDataPtr drawingData);

		static unsigned int RegisterTexture(const TexturePtr texture);
        static void UnregisterTexture(unsigned int textureIndex, GLenum type);
        static void DestroyMaterial(unsigned int materialId);
        static void DestroyRenderUnit(RenderUnitPtr renderUnit, DrawingDataPtr drawingData);
        static void DestroyShaderProgram(unsigned int programId);

		static void UpdateTextures();
        static void UpdateTextureType(GLenum type);
    private:
        static void AllocateTextureTypes();
        static void AllocateTextureType(GLenum type);

    public:
        static void Draw();
        static void Draw(RenderUnit& renderUnit);

		static std::unordered_map<GLuint, DrawingDataPtr> m_drawingDatas;
		static std::unordered_map<unsigned int, ShaderProgramPtr> m_shaderPrograms;
		static std::unordered_map<unsigned int, MaterialPtr> m_materials;
		static std::unordered_map<GLenum, std::vector<TexturePtr>> m_texturesByType;
        static std::unordered_map<std::string, TexturePtr> m_texturesCache;

        static std::unordered_map<GLenum, std::vector<GLuint64>> m_handlesByType;
        static std::unordered_map<GLenum, GLuint> m_buffersByType;
        static std::unordered_map<GLenum, int> m_bindingsByType;

        static std::vector<glm::mat4> matrices;

        //static std::vector<uvec2>
    private:
        static bool texturesUploaded;
        static RendererConfig m_config;
        static GLFWwindow* m_window;
    };

}
