#pragma once

#include "Cubemap.h"
#include "FileFunctions.h"
// #include "RenderUnit.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "Texture2D.h"
// #include "Texture2D.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace RuamEngine
{
    inline std::string diffuseTexDefaultPath = "RuamCore/Assets/Sprites/DefaultSprite.png";
    inline std::string specularTexDefaultPath = "RuamCore/Assets/Sprites/DefaultSprite.png";
    inline std::string reflectionTexDefaultPath = "RuamCore/Assets/Sprites/DefaultSprite.png";
    inline std::string skyboxDefaultPath = "RuamCore/Assets/Sprites/Skybox.png";

    inline std::string generalVertexShaderDefaultPath = "RuamCore/Rendering/Shaders/GeneralVertexShader.glsl";
    inline std::string generalFragmentShaderDefaultPath = "RuamCore/Rendering/Shaders/GeneralFragmentShader.glsl";
    inline std::string skyboxVertexShaderDefaultPath = "RuamCore/Rendering/Shaders/SkyboxVertexShader.glsl";
    inline std::string skyboxFragmentShaderDefaultPath = "RuamCore/Rendering/Shaders/SkyboxFragmentShader.glsl";

    class ResourceManager
    {
    public:
        static void Init();
        static void Shutdown();

        // Textures handling ---------------------------------------------------------------------------------
        static Texture2DSPtr LoadTexture2D(const std::string& relativePath);

        static Texture2DSPtr LoadTexture2D(const std::string& modelTexPath, const aiTexture* tex);

        static CubemapSPtr LoadCubemap(const std::vector<std::string>& relativePaths);
        static CubemapSPtr LoadCubemap(const std::string& relativePath);

        static TextureSPtr GetTexture2D(const std::string& relativePath);
        static CubemapSPtr GetCubemap(const std::vector<std::string>& relativePaths);
        static CubemapSPtr GetCubemap(const std::string& relativePath);
        static void RemoveTexture2DIfExpired(const std::string& relativePath);
        static void RemoveCubemapIfExpired(const std::vector<std::string>& relativePaths);
        static void RemoveCubemapIfExpired(const std::string& relativePath);

        // Model handling ---------------------------------------------------------------------------------
        static ModelSPtr LoadModel(const std::string& relativePath);
        static ModelSPtr GetModel(const std::string& relativePath);
        static void RemoveModelIfExpired(const std::string& relativePath);
        // Material handling ---------------------------------------------------------------------------------
        static MaterialSPtr CreateMaterial(Texture2DSPtr diffuse, Texture2DSPtr specular, Texture2DSPtr reflection);
        static MaterialSPtr GetMaterial(unsigned int materialId);
        static void RemoveMaterialIfExpired(unsigned int materialId);

        // Shader Program handling ---------------------------------------------------------------------------------
        static ShaderProgramSPtr LoadShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        static ShaderProgramSPtr GetShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        static ShaderProgramSPtr GetShaderProgram(const std::string& shaderProgramName);
        static void RemoveShaderProgramIfExpired(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    private:

public:
        static std::unordered_map<std::string, Texture2DWPtr> s_texture2DCache;
        static std::unordered_map<std::string, CubemapWPtr> s_cubemapsCache;
        static std::unordered_map<std::string, ModelWPtr> s_modelCache;
        static std::unordered_map<unsigned int, MaterialWPtr> s_materialCache;
        static std::unordered_map<ShaderProgramName, ShaderProgramWPtr> s_shaderProgramsCache;
        friend class Material;
        friend class Texture2D;
        friend class Cubemap;
    };
}
