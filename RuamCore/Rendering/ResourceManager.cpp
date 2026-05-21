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
    std::unordered_map<std::string, ResourceManager::ModelEntry> ResourceManager::m_modelCache;

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

    ModelWPtr ResourceManager::LoadModel(const std::string& relativePath, ShaderProgramType shaderProgramType)
    {
        auto it = m_modelCache.find(relativePath);
        if (it != m_modelCache.end())
        {
            it->second.refCount[shaderProgramType]++;
            return it->second.model;
        }

        ModelSPtr newModel = std::make_shared<Model>(relativePath);

        ModelEntry newEntry;
        newEntry.refCount[shaderProgramType]++;
        newEntry.model = newModel;

        m_modelCache[relativePath] = newEntry;

        return newModel;
    }
    void ResourceManager::UnloadModel(const std::string& relativePath, ShaderProgramType shaderProgramType)
    {
	    auto it = m_modelCache.find(relativePath);
	    if (it == m_modelCache.end())
	    {
	        std::cout << "Warning: There is not such a model with path " << relativePath << " to unload\n";
	        return;
	    }
	    it->second.refCount[shaderProgramType]--;

		if (it->second.refCount[shaderProgramType] <=0)
        {
       		DrawingDataSPtr drawingData = Renderer::s_drawingDatas[shaderProgramType];
        	std::set<RenderUnitSPtr> unitsToDestroy;
         	for (const MeshSPtr& mesh : it->second.model->m_meshes)
			{
				RenderUnitSPtr ru = Renderer::GetRenderUnit(mesh->m_material, shaderProgramType);
				if (ru != nullptr)
				{
					unitsToDestroy.insert(ru);

				}
			}
			for (auto& ru : unitsToDestroy)
			{
           		Renderer::DestroyRenderUnit(ru, drawingData);
			}
        }

		bool noMoreReferences = true;
		for (auto [type, count] : m_modelCache[relativePath].refCount)
		{
			if (count>0) noMoreReferences = false;
		}
		if (noMoreReferences) m_modelCache.erase(it);
    }
    ModelWPtr ResourceManager::GetModel(const std::string& relativePath)
    {
	   	auto it = m_modelCache.find(relativePath);
		if (it != m_modelCache.end())
	    {
	        return it->second.model;
	    }
		return {};
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
