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
    std::unordered_map<unsigned int, MaterialWPtr> ResourceManager::m_materialCache;
    std::unordered_map<std::string, TextureWPtr> ResourceManager::m_textureCache;
    std::unordered_map<std::string, ModelWPtr> ResourceManager::m_modelCache;

    void ResourceManager::Init()
    {
        LoadTexture<Texture2D>("RuamCore/Assets/Sprites/DefaultSprite.png");
    }

    // Texture handling ---------------------------------------------------------------------------------

    // Returns nullptr if the texture is not found
    TextureSPtr ResourceManager::GetTexture(const std::string &relativePath)
    {
        auto it = m_textureCache.find(relativePath);
        if (it != m_textureCache.end()) return it->second.lock();
        return nullptr;
    }

    // Returns nullptr if the texture is not found
    TextureSPtr ResourceManager::GetTexture(const std::vector<std::string>& relativePaths)
    {
        std::string unifiedPath = UnifyPaths(relativePaths);
        auto it = m_textureCache.find(unifiedPath);
        if (it != m_textureCache.end()) return it->second.lock();
        return nullptr;
    }

    // Model handling ---------------------------------------------------------------------------------

    ModelSPtr ResourceManager::LoadModel(const std::string& relativePath)
    {
        auto it = m_modelCache.find(relativePath);
        if (it != m_modelCache.end())
        {
            if (!it->second.expired()) return it->second.lock();
        }

        ModelSPtr newModel = std::make_shared<Model>(relativePath);

        m_modelCache[relativePath] = newModel;

        return newModel;
    }
    ModelSPtr ResourceManager::GetModel(const std::string& relativePath)
    {
	   	auto it = m_modelCache.find(relativePath);
		if (it != m_modelCache.end())
	    {
			if (!it->second.expired()) return it->second.lock();
	    }
		return nullptr;
    }

    // Material handling ---------------------------------------------------------------------------------

    MaterialSPtr ResourceManager::CreateMaterial(const std::string& diffuseTexPath, const std::string& specularTexPath, const std::string& reflectionTexPath)
    {
        MaterialSPtr newMaterial = std::make_shared<Material>();
        newMaterial->m_diffuseTexture = LoadTexture<Texture2D>(diffuseTexPath);
        newMaterial->m_specularTexture = LoadTexture<Texture2D>(specularTexPath);
        newMaterial->m_reflectionTexture = LoadTexture<Texture2D>(reflectionTexPath);

        m_materialCache[newMaterial->id()] = newMaterial;
		return newMaterial;
    }
    MaterialSPtr ResourceManager::GetMaterial(unsigned int materialId)
    {
        auto it = m_materialCache.find(materialId);
        if (it != m_materialCache.end())
        {
            if (!it->second.expired()) return it->second.lock();
        }
        else return nullptr;
    }

    // Shader Program handling ---------------------------------------------------------------------------------
    ShaderProgramSPtr ResourceManager::CreateShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        ShaderProgramSPtr newProgram = std::make_shared<ShaderProgram>(vertexShaderPath, fragmentShaderPath);
		return newProgram;
    }
}
