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
    std::unordered_map<unsigned int, ResourceManager::MaterialEntry> ResourceManager::m_materialCache;
    std::unordered_map<std::string, ResourceManager::TextureEntry> ResourceManager::m_textureCache;
    std::unordered_map<std::string, ResourceManager::ModelEntry> ResourceManager::m_modelCache;

    void ResourceManager::Init()
    {
        LoadTexture<Texture2D>("RuamCore/Assets/Sprites/DefaultSprite.png");
        std::vector<std::string> skyboxes =
			{
			"RuamCore/Assets/Sprites/Skybox.png","RuamCore/Assets/Sprites/Skybox.png","RuamCore/Assets/Sprites/Skybox.png","RuamCore/Assets/Sprites/Skybox.png","RuamCore/Assets/Sprites/Skybox.png","RuamCore/Assets/Sprites/Skybox.png"
			};
        LoadTexture<Cubemap>(skyboxes);
    }

    // Texture handling ---------------------------------------------------------------------------------

    // Returns nullptr if the texture is not found
    TextureWPtr ResourceManager::GetTexture(const std::string &relativePath)
    {
        auto it = m_textureCache.find(relativePath);
        if (it != m_textureCache.end()) return it->second.texture;
        return {};
    }

    // Returns nullptr if the texture is not found
    TextureWPtr ResourceManager::GetTexture(const std::vector<std::string>& relativePaths)
    {
        std::string unifiedPath = UnifyPaths(relativePaths);
        auto it = m_textureCache.find(unifiedPath);
        if (it != m_textureCache.end()) return it->second.texture;
        return {};
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

        Renderer::UpdateTextureType(GL_TEXTURE_2D);
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
       		DrawingDataSPtr drawingData = Renderer::m_drawingDatas[shaderProgramType];
        	std::set<RenderUnitSPtr> unitsToDestroy;
         	std::set<std::string> texturesToDestroy;
         	for (const MeshSPtr& mesh : it->second.model->m_meshes)
			{
				RenderUnitSPtr ru = Renderer::GetRenderUnit(mesh->m_material, shaderProgramType);
				if (ru != nullptr)
				{
					unitsToDestroy.insert(ru);

				}
				MaterialSPtr mat = GetShared(mesh->m_material);
				if (auto diffuseTex = mat->m_diffuseTexture.lock()) texturesToDestroy.insert(diffuseTex->path());
				if (auto specularTex = mat->m_specularTexture.lock()) texturesToDestroy.insert(specularTex->path());
				if (auto reflectionTex = mat->m_reflectionTexture.lock()) texturesToDestroy.insert(reflectionTex->path());
			}
			for (auto& ru : unitsToDestroy)
			{
           		Renderer::DestroyRenderUnit(ru, drawingData);
			}
			for (auto& path : texturesToDestroy)
			{
				UnloadTexture<Texture2D>(path);
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

    MaterialWPtr ResourceManager::CreateMaterial()
    {
   		MaterialSPtr newMaterial = std::make_shared<Material>();
     	MaterialEntry newEntry;
     	newEntry.material = newMaterial;
     	newEntry.refCount++;
		m_materialCache[newMaterial->id()] = newEntry;
		return newMaterial;
    }
    void ResourceManager::DestroyMaterial(unsigned int materialId)
    {
    	auto it = m_materialCache.find(materialId);
     	if (it == m_materialCache.end())
		{
			std::cout << "Warning: There is not such a material with id " << materialId << " to destroy\n";
            return;
		}
      	it->second.refCount--;

       	if (it->second.refCount<=0)
        {
       		m_materialCache.erase(materialId);
        }

    }
    MaterialWPtr ResourceManager::GetMaterial(unsigned int materialId)
    {
   		auto it = m_materialCache.find(materialId);
    	if (it == m_materialCache.end())
		{
			std::cout << "Warning: There is not such a material with id " << materialId << " to destroy\n";
           return {};
		}
     	it->second.refCount++;
    	return it->second.material;
    }
}
