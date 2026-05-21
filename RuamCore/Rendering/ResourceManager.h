#pragma once

#include "Cubemap.h"
#include "FileFunctions.h"
// #include "RenderUnit.h"
#include "Renderer.h"
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
            auto it = m_textureCache.find(relativePath);
            if (it != m_textureCache.end())
            {
                if (!it->second.expired()) return it->second.lock();
                else m_textureCache.erase(it);
            }

            TextureSPtr newTexture = std::make_shared<T>(relativePath);

            m_textureCache[relativePath] = newTexture;
            return newTexture;
        }
        template <typename T>
        static TextureSPtr LoadTexture(const std::vector<std::string>& relativePaths)
        {
            std::string unifiedPath = UnifyPaths(relativePaths);
            auto it = m_textureCache.find(unifiedPath);
            if (it != m_textureCache.end())
            {
                if (!it->second.expired()) return it->second.lock();
                else m_textureCache.erase(it);
            }

            TextureSPtr newTexture = std::make_shared<T>(relativePaths);
            m_textureCache[unifiedPath] = newTexture;
            return newTexture;
        }

        static TextureSPtr GetTexture(const std::string& relativePath);
        static TextureSPtr GetTexture(const std::vector<std::string>& relativePaths);

        // Model handling ---------------------------------------------------------------------------------
        static ModelWPtr LoadModel(const std::string& relativePath, ShaderProgramType shaderProgramType);
        static void UnloadModel(const std::string& relativePath, ShaderProgramType shaderProgramType);
        static ModelWPtr GetModel(const std::string& relativePath);

        // Material handling ---------------------------------------------------------------------------------
        static MaterialSPtr CreateMaterial(const std::string& diffuseTexPath, const std::string& specularTexPath = diffuseTexDefaultPath, const std::string& reflectionTexPath = reflectionTexDefaultPath);
        static MaterialSPtr GetMaterial(unsigned int materialId);

        // Shader Program handling ---------------------------------------------------------------------------------
        static ShaderProgramSPtr CreateShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    private:

        struct ModelEntry
        {
        	ModelSPtr model;
         	std::unordered_map<ShaderProgramType, int> refCount;
        };

public:
        static std::unordered_map<std::string, TextureWPtr> m_textureCache;
        static std::unordered_map<std::string, ModelEntry> m_modelCache;
        static std::unordered_map<unsigned int, MaterialWPtr> m_materialCache;

        friend class Material;
        friend class Texture2D;
        friend class Cubemap;
    };
}
