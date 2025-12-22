#pragma once

#include "FileFunctions.h"
#include "RenderUnit.h"
#include "Renderer.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "Texture2D.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace RuamEngine
{
    class ResourceManager
    {
    public:
        static void Init();
        static void Shutdown();


        // Textures handling ---------------------------------------------------------------------------------
        template <typename T>
        static TexturePtr LoadTexture(const std::string& relativePath)
        {
            auto it = m_textureCache.find(relativePath);
            if (it != m_textureCache.end())
            {
                it->second.refCount++;
                return it->second.texture;
            }

            TexturePtr newTexture = std::make_shared<T>(relativePath);
            unsigned int rendererIdx = RegisterTextureInRenderer(newTexture);

            TextureEntry newEntry;
            newEntry.rendererIndex = rendererIdx;
            newEntry.texture = newTexture;
            newEntry.refCount = 1;

            m_textureCache[relativePath] = newEntry;
            return newTexture;
        }
        template <typename T>
        static TexturePtr LoadTexture(const std::vector<std::string>& relativePaths)
        {
            std::string unifiedPath = UnifyPaths(relativePaths);
            auto it = m_textureCache.find(unifiedPath);
            if (it != m_textureCache.end())
            {
                it->second.refCount++;
                return it->second.texture;
            }

            TexturePtr newTexture = std::make_shared<T>(relativePaths);
            unsigned int rendererIdx = RegisterTextureInRenderer(newTexture);

            TextureEntry newEntry;
            newEntry.rendererIndex = rendererIdx;
            newEntry.texture = newTexture;
            newEntry.refCount = 1;

            m_textureCache[unifiedPath] = newEntry;
            return newTexture;
        }

        template<typename T>
        static void UnloadTexture(const std::string& relativePath)
        {
            auto it = m_textureCache.find(relativePath);
            if (it == m_textureCache.end())
            {
                std::cout << "Warning: There is not such a texture with path " << relativePath << " to unload\n";
                return;
            }
            it->second.refCount--;

            if (it->second.refCount <=0)
            {
                UnregisterTextureInRenderer(it->second.rendererIndex, it->second.texture->GetType());
                m_textureCache.erase(it);
            }

        }
        static TexturePtr GetTexture(const std::string& relativePath);
        static TexturePtr GetTexture(const std::vector<std::string>& relativePaths);

        // Model handling ---------------------------------------------------------------------------------
        static ModelPtr LoadModel(const std::string& relativePath, ShaderProgramType shaderProgramType);
        static void UnloadModel(const std::string& relativePath, ShaderProgramType shaderProgramType);
        static ModelPtr GetModel(const std::string& relativePath);

        static unsigned int RegisterTextureInRenderer(TexturePtr texture);
        static void UnregisterTextureInRenderer(unsigned int textureIndex, GLenum type);
        static void DestroyRenderUnitInRenderer(RenderUnitPtr renderUnit, DrawingDataPtr drawingData);
    private:

        struct TextureEntry
        {
            TexturePtr texture;
            int refCount = 0;
            unsigned int rendererIndex = 0; // No the OpenGL ID, but the index in the Renderer vector
        };

        struct ModelEntry
        {
        	ModelPtr model;
         	std::map<ShaderProgramType, int> refCount;
        };

        static std::unordered_map<unsigned int, MaterialPtr> m_materials;
        static std::unordered_map<std::string, TextureEntry> m_textureCache;
        static std::unordered_map<std::string, ModelEntry> m_modelCache;
    };
}
