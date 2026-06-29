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
        template <typename T>
        static TextureSPtr LoadTexture(const std::string& relativePath)
        {
            auto it = s_textureCache.find(relativePath);
            if (it != s_textureCache.end())
            {
                if (!it->second.expired()) return it->second.lock();
                else s_textureCache.erase(it);
            }

            TextureSPtr newTexture = std::make_shared<T>(relativePath);

            s_textureCache[relativePath] = newTexture;
            return newTexture;
        }
        template <typename T>
        static TextureSPtr LoadTexture(const std::vector<std::string>& relativePaths)
        {
            std::string unifiedPath = unifyPaths(relativePaths);
            auto it = s_textureCache.find(unifiedPath);
            if (it != s_textureCache.end())
            {
                if (!it->second.expired()) return it->second.lock();
                else s_textureCache.erase(it);
            }

            TextureSPtr newTexture = std::make_shared<T>(relativePaths);
            s_textureCache[unifiedPath] = newTexture;
            return newTexture;
        }

        static TextureSPtr GetTexture(const std::string& relativePath);
        static TextureSPtr GetTexture(const std::vector<std::string>& relativePaths);
        static void RemoveTextureIfExpired(const std::string& relativePath);
        static void RemoveTextureIfExpired(const std::vector<std::string>& relativePaths);

        // Model handling ---------------------------------------------------------------------------------
        static ModelSPtr LoadModel(const std::string& relativePath);
        static ModelSPtr GetModel(const std::string& relativePath);
        static void RemoveModelIfExpired(const std::string& relativePath);
        // Material handling ---------------------------------------------------------------------------------
        static MaterialSPtr CreateMaterial(const std::string& diffuseTexPath, const std::string& specularTexPath = diffuseTexDefaultPath, const std::string& reflectionTexPath = reflectionTexDefaultPath);
        static MaterialSPtr GetMaterial(unsigned int materialId);
        static void RemoveMaterialIfExpired(unsigned int materialId);

        // Shader Program handling ---------------------------------------------------------------------------------
        static ShaderProgramSPtr LoadShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        static ShaderProgramSPtr GetShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        static ShaderProgramSPtr GetShaderProgram(const std::string& shaderProgramName);
        static void RemoveShaderProgramIfExpired(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    private:

public:
        static std::unordered_map<std::string, TextureWPtr> s_textureCache;
        static std::unordered_map<std::string, ModelWPtr> s_modelCache;
        static std::unordered_map<unsigned int, MaterialWPtr> s_materialCache;
        static std::unordered_map<ShaderProgramName, ShaderProgramWPtr> s_shaderProgramsCache;
        friend class Material;
        friend class Texture2D;
        friend class Cubemap;
    };
}
