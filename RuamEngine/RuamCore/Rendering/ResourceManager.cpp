#include "ResourceManager.h"
#include "FileFunctions.h"
#include "Model.h"
#include "RenderUnit.h"
#include "Renderer.h"
#include <memory>
#include <set>
namespace RuamEngine
{
    std::unordered_map<unsigned int, MaterialPtr> ResourceManager::m_materials;
    std::unordered_map<std::string, ResourceManager::TextureEntry> ResourceManager::m_textureCache;
    std::unordered_map<std::string, ResourceManager::ModelEntry> ResourceManager::m_modelCache;

    void ResourceManager::Init()
    {
        LoadTexture<Texture2D>("assets/sprites/defaultSprite.png");
        std::vector<std::string> skyboxes =
			{
			"assets/sprites/skybox.png","assets/sprites/skybox.png","assets/sprites/skybox.png","assets/sprites/skybox.png","assets/sprites/skybox.png","assets/sprites/skybox.png"
			};
        LoadTexture<Cubemap>(skyboxes);
    }

    // Texture handling ---------------------------------------------------------------------------------

    // Returns nullptr if the texture is not found
    TexturePtr ResourceManager::GetTexture(const std::string &relativePath)
    {
        auto it = m_textureCache.find(relativePath);
        if (it != m_textureCache.end()) return it->second.texture;
        return nullptr;
    }

    // Returns nullptr if the texture is not found
    TexturePtr ResourceManager::GetTexture(const std::vector<std::string>& relativePaths)
    {
        std::string unifiedPath = UnifyPaths(relativePaths);
        auto it = m_textureCache.find(unifiedPath);
        if (it != m_textureCache.end()) return it->second.texture;
        return nullptr;
    }

    // Model handling ---------------------------------------------------------------------------------

    ModelPtr ResourceManager::LoadModel(const std::string& relativePath, ShaderProgramType shaderProgramType)
    {
        auto it = m_modelCache.find(relativePath);
        if (it != m_modelCache.end())
        {
            it->second.refCount[shaderProgramType]++;
            return it->second.model;
        }

        ModelPtr newModel = std::make_shared<Model>(relativePath);

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
       		DrawingDataPtr drawingData = Renderer::m_drawingDatas[shaderProgramType];
        	std::set<RenderUnitPtr> unitsToDestroy;
         	for (Mesh mesh : it->second.model->m_meshes)
			{
				int index = Renderer::FindRenderUnit(mesh.m_material, drawingData);
				if (index !=-1)
				{
					unitsToDestroy.insert(drawingData->m_renderUnits[index]);
				}
			}
			for (auto& ru : unitsToDestroy)
			{
           		DestroyRenderUnitInRenderer(ru, drawingData);
			}
        }

		bool noMoreReferences = true;
		for (auto [type, count] : m_modelCache[relativePath].refCount)
		{
			if (count>0) noMoreReferences = false;
		}
		if (noMoreReferences) m_modelCache.erase(it);
    }
    // ModelPtr ResourceManager::GetModel(const std::string& relativePath)
    // {

    // }


    unsigned int ResourceManager::RegisterTextureInRenderer(TexturePtr texture)
    {
        return Renderer::RegisterTexture(texture);
    }

    void ResourceManager::UnregisterTextureInRenderer(unsigned int textureIndex, GLenum type)
    {
        Renderer::UnregisterTexture(textureIndex, type);
    }
    void ResourceManager::DestroyRenderUnitInRenderer(RenderUnitPtr renderUnit, DrawingDataPtr drawingData)
    {
        Renderer::DestroyRenderUnit(renderUnit, drawingData);
    }
}
