#include "ResourceManager.h"
#include "FileFunctions.h"
#include "Model.h"
#include "Cubemap.h"
#include "Renderer.h"
#include "RuamUtils.h"
#include <memory>
#include <set>
namespace RuamEngine
{
    std::unordered_map<std::string, Texture2DWPtr> ResourceManager::s_texture2DCache = {};
    std::unordered_map<std::string, CubemapWPtr> ResourceManager::s_cubemapsCache = {};
    std::unordered_map<std::string, ModelWPtr> ResourceManager::s_modelCache = {};
    std::unordered_map<unsigned int, MaterialWPtr> ResourceManager::s_materialCache = {};
    std::unordered_map<ShaderProgramName, ShaderProgramWPtr> ResourceManager::s_shaderProgramsCache = {};
    void ResourceManager::Init()
    {
        LoadTexture2D("RuamCore/Assets/Sprites/DefaultSprite.png");
    }

    // Texture handling ---------------------------------------------------------------------------------

    Texture2DSPtr ResourceManager::LoadTexture2D(const std::string& relativePath)
    {
        auto it = s_texture2DCache.find(relativePath);
        if (it != s_texture2DCache.end())
        {
            if (!it->second.expired()) return it->second.lock();
            else s_texture2DCache.erase(it);
        }

        Texture2DSPtr newTexture = std::make_shared<Texture2D>(relativePath);

        s_texture2DCache[relativePath] = newTexture;
        return newTexture;
    }
    Texture2DSPtr ResourceManager::LoadTexture2D(const std::string& modelTexPath, const aiTexture* tex)
    {
        auto it = s_texture2DCache.find(modelTexPath);
        if (it != s_texture2DCache.end())
        {
            if (!it->second.expired()) return GetShared<Texture2D>(it->second);
            else s_texture2DCache.erase(it);
        }

        Texture2DSPtr newTexture = std::make_shared<Texture2D>(modelTexPath, tex);

        s_texture2DCache[modelTexPath] = newTexture;
        return newTexture;
    }

    CubemapSPtr ResourceManager::LoadCubemap(const std::string& relativePath)
    {
        auto it = s_cubemapsCache.find(relativePath);
        if (it != s_cubemapsCache.end())
        {
            if (!it->second.expired()) return it->second.lock();
            else s_cubemapsCache.erase(it);
        }

        CubemapSPtr newCubemap = std::make_shared<Cubemap>(relativePath);
        s_cubemapsCache[relativePath] = newCubemap;
        return newCubemap;
    }
    CubemapSPtr ResourceManager::LoadCubemap(const std::vector<std::string>& relativePaths)
    {
        std::string unifiedPath = unifyPaths(relativePaths);
        auto it = s_cubemapsCache.find(unifiedPath);
        if (it != s_cubemapsCache.end())
        {
            if (!it->second.expired()) return it->second.lock();
            else s_cubemapsCache.erase(it);
        }

        CubemapSPtr newCubemap = std::make_shared<Cubemap>(relativePaths);
        s_cubemapsCache[unifiedPath] = newCubemap;
        return newCubemap;
    }

    // Returns nullptr if the texture is not found
    TextureSPtr ResourceManager::GetTexture2D(const std::string &relativePath)
    {
        auto it = s_texture2DCache.find(relativePath);
        if (it != s_texture2DCache.end()) return it->second.lock();
        return nullptr;
    }

    // Returns nullptr if the texture is not found
    CubemapSPtr ResourceManager::GetCubemap(const std::vector<std::string>& relativePaths)
    {
        std::string unifiedPath = unifyPaths(relativePaths);
        auto it = s_cubemapsCache.find(unifiedPath);
        if (it != s_cubemapsCache.end()) return it->second.lock();
        return nullptr;
    }

    CubemapSPtr ResourceManager::GetCubemap(const std::string& relativePath)
    {
        auto it = s_cubemapsCache.find(relativePath);
        if (it != s_cubemapsCache.end()) return it->second.lock();
        return nullptr;
    }

    void ResourceManager::RemoveTexture2DIfExpired(const std::string& relativePath)
    {
        auto it = s_texture2DCache.find(relativePath);

        if (it != s_texture2DCache.end() && it->second.expired())
        {
            s_texture2DCache.erase(relativePath);
            std::cout << "Texture2D of relative path " << relativePath << " destroyed\n";
        }
    }
    void ResourceManager::RemoveCubemapIfExpired(const std::vector<std::string>& relativePaths)
    {
        std::string relativePath = unifyPaths(relativePaths);
        auto it = s_cubemapsCache.find(relativePath);

        if (it != s_cubemapsCache.end() && it->second.expired())
        {
            s_cubemapsCache.erase(relativePath);
            std::cout << "Cubemap of unified path " << relativePath << " destroyed\n";
        }
    }

    void ResourceManager::RemoveCubemapIfExpired(const std::string& relativePath)
    {
        auto it = s_cubemapsCache.find(relativePath);

        if (it != s_cubemapsCache.end() && it->second.expired())
        {
            s_cubemapsCache.erase(relativePath);
            std::cout << "Cubemap of path " << relativePath << " destroyed\n";
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

    MaterialSPtr ResourceManager::CreateMaterial(Texture2DSPtr diffuse, Texture2DSPtr specular, Texture2DSPtr reflection)
    {
        MaterialSPtr newMaterial = std::make_shared<Material>(diffuse, specular, reflection);

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
        return nullptr;
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

    ShaderProgramSPtr ResourceManager::LoadShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        std::string unifiedPath = unifyPaths({vertexShaderPath, fragmentShaderPath});
        auto it = s_shaderProgramsCache.find(unifiedPath);
        if (it != s_shaderProgramsCache.end())
        {
            if (!it->second.expired()) return it->second.lock();
        }
        ShaderProgramSPtr newShaderProgram = std::make_shared<ShaderProgram>(vertexShaderPath, fragmentShaderPath);
        s_shaderProgramsCache[unifiedPath] = newShaderProgram;
        return newShaderProgram;
    }
    ShaderProgramSPtr ResourceManager::GetShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        std::string unifiedPath = unifyPaths({vertexShaderPath, fragmentShaderPath});
        auto it = s_shaderProgramsCache.find(unifiedPath);
        if (it != s_shaderProgramsCache.end())
        {
            if (!it->second.expired()) return it->second.lock();
        }
        return nullptr;
    }
    ShaderProgramSPtr ResourceManager::GetShaderProgram(const std::string& shaderProgramName)
    {
        auto it = s_shaderProgramsCache.find(shaderProgramName);
        if (it != s_shaderProgramsCache.end())
        {
            if (!it->second.expired()) return it->second.lock();
        }
        return nullptr;
    }
    void ResourceManager::RemoveShaderProgramIfExpired(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        std::string unifiedPath = unifyPaths({vertexShaderPath, fragmentShaderPath});
        auto it = s_shaderProgramsCache.find(unifiedPath);

        if (it != s_shaderProgramsCache.end() && it->second.expired())
        {
            s_shaderProgramsCache.erase(unifiedPath);
            std::cout << "Shader Program destroyed. Made of\n vertex shader: " << vertexShaderPath << "\n fragment shader: " << fragmentShaderPath << "\n";
        }
    }
}
