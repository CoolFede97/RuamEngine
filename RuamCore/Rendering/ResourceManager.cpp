#include "ResourceManager.h"
#include "FileFunctions.h"
#include "Model.h"
#include "RenderUnit.h"
#include "Cubemap.h"
#include "Renderer.h"
#include "RuamUtils.h"
#include <memory>
#include <set>
namespace RuamEngine
{
    std::unordered_map<unsigned int, MaterialWPtr> ResourceManager::s_materialCache;
    std::unordered_map<std::string, TextureWPtr> ResourceManager::s_textureCache;
    std::unordered_map<std::string, ModelWPtr> ResourceManager::s_modelCache;

    void ResourceManager::Init()
    {
        LoadTexture<Texture2D>("RuamCore/Assets/Sprites/DefaultSprite.png");
    }

    // Texture handling ---------------------------------------------------------------------------------

    // Returns nullptr if the texture is not found
    TextureSPtr ResourceManager::GetTexture(const std::string &relativePath)
    {
        auto it = s_textureCache.find(relativePath);
        if (it != s_textureCache.end()) return it->second.lock();
        return nullptr;
    }

    // Returns nullptr if the texture is not found
    TextureSPtr ResourceManager::GetTexture(const std::vector<std::string>& relativePaths)
    {
        std::string unifiedPath = UnifyPaths(relativePaths);
        auto it = s_textureCache.find(unifiedPath);
        if (it != s_textureCache.end()) return it->second.lock();
        return nullptr;
    }

    void ResourceManager::RemoveTextureIfExpired(const std::string& relativePath)
    {
        auto it = s_textureCache.find(relativePath);

        if (it != s_textureCache.end() && it->second.expired())
        {
            s_textureCache.erase(relativePath);
            std::cout << "Texture of relative path " << relativePath << " destroyed\n";
        }
    }
    void ResourceManager::RemoveTextureIfExpired(const std::vector<std::string>& relativePaths)
    {
        std::string relativePath = UnifyPaths(relativePaths);
        auto it = s_textureCache.find(relativePath);

        if (it != s_textureCache.end() && it->second.expired())
        {
            s_textureCache.erase(relativePath);
            std::cout << "Texture of relative path " << relativePath << " destroyed\n";
        }
    }

    // Model handling ---------------------------------------------------------------------------------

    ModelSPtr ResourceManager::LoadModel(const std::string& relativePath)
    {
        auto it = s_modelCache.find(relativePath);
        if (it != s_modelCache.end())
        {
            if (!it->second.expired()) return it->second.lock();
        }

        ModelSPtr newModel = std::make_shared<Model>(relativePath);

        s_modelCache[relativePath] = newModel;

        return newModel;
    }
    ModelSPtr ResourceManager::GetModel(const std::string& relativePath)
    {
	   	auto it = s_modelCache.find(relativePath);
		if (it != s_modelCache.end())
	    {
			if (!it->second.expired()) return it->second.lock();
	    }
		return nullptr;
    }
    void ResourceManager::RemoveModelIfExpired(const std::string& relativePath)
    {
        auto it = s_modelCache.find(relativePath);

        if (it != s_modelCache.end() && it->second.expired())
        {
            s_modelCache.erase(relativePath);
            std::cout << "Model of relative " << relativePath << " destroyed\n";
        }
    }
    // Material handling ---------------------------------------------------------------------------------

    MaterialSPtr ResourceManager::CreateMaterial(const std::string& diffuseTexPath, const std::string& specularTexPath, const std::string& reflectionTexPath)
    {
        MaterialSPtr newMaterial = std::make_shared<Material>();
        newMaterial->m_diffuseTexture = LoadTexture<Texture2D>(diffuseTexPath);
        newMaterial->m_specularTexture = LoadTexture<Texture2D>(specularTexPath);
        newMaterial->m_reflectionTexture = LoadTexture<Texture2D>(reflectionTexPath);

        s_materialCache[newMaterial->id()] = newMaterial;
		return newMaterial;
    }
    MaterialSPtr ResourceManager::GetMaterial(unsigned int materialId)
    {
        auto it = s_materialCache.find(materialId);
        if (it != s_materialCache.end())
        {
            if (!it->second.expired()) return it->second.lock();
        }
        else return nullptr;
    }
    void ResourceManager::RemoveMaterialIfExpired(unsigned int materialId)
    {
        auto it = s_materialCache.find(materialId);

        if (it != s_materialCache.end() && it->second.expired())
        {
            s_materialCache.erase(materialId);
            std::cout << "Material of id " << materialId << " destroyed\n";
        }
    }
    // Shader Program handling ---------------------------------------------------------------------------------
    ShaderProgramSPtr ResourceManager::CreateShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        ShaderProgramSPtr newProgram = std::make_shared<ShaderProgram>(vertexShaderPath, fragmentShaderPath);
		return newProgram;
    }
}
