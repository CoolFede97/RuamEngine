#include "ModelRenderer.h"
#include "ResourceManager.h"

namespace RuamEngine
{
    ModelRenderer::~ModelRenderer()
   	{
   		if (!m_meshPath.empty())
   		{
   			ResourceManager::UnloadModel(m_meshPath, m_shaderProgramType);
   			m_cachedRenderUnits.clear();
   		}
   	}
    void ModelRenderer::update()
	{
       	glm::mat4 modelMatrix(1.0f);

       	glm::vec3 parentsPos = {0,0,0};
       	glm::vec3 parentsRotation = {0,0,0};
       	glm::vec3 parentsScale = {1,1,1};
       	Transform* lastParent = nullptr;
       	if (transform().m_parent != nullptr)
       	{
       		lastParent = transform().m_parent;
       		while (true)
       		{
       			if (lastParent==nullptr) break;
       			parentsPos+=lastParent->position();
       			parentsRotation+=lastParent->rotation();
       			parentsScale*=lastParent->scale();
       			lastParent = lastParent->m_parent;
       		}
       	}

       	modelMatrix = glm::translate(modelMatrix, entity()->transform().position() + parentsPos);
       	modelMatrix = glm::rotate(modelMatrix, glm::radians(entity()->transform().rotation().x + parentsRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
       	modelMatrix = glm::rotate(modelMatrix, glm::radians(entity()->transform().rotation().y + parentsRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
       	modelMatrix = glm::rotate(modelMatrix, glm::radians(entity()->transform().rotation().z + parentsRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
       	modelMatrix = glm::scale(modelMatrix, entity()->transform().scale() * parentsScale);

       	ModelSPtr modelShared = GetShared(m_model);

           if (modelShared->m_localToGlobalMaterials.size() == 1)
       	{
       		MeshSPtr mesh = modelShared->m_meshes[0];
       		RenderUnitSPtr ru = m_cachedRenderUnits[mesh->m_material.lock()->id()].lock();
       		if (ru != nullptr)
       		{
       			ru->m_modelMatricesBuffer->addBatchData({ modelMatrix });
       			return;
       		}
       	}

       	std::vector<unsigned int> renderUnitsUsed = {};
       	for (const MeshSPtr& mesh : m_model.lock()->m_meshes)
       	{
       		RenderUnitSPtr ru = m_cachedRenderUnits[mesh->m_material.lock()->id()].lock();

       		MaterialSPtr ruSharedMaterial = GetShared(ru->m_material);
       		std::vector<unsigned int>::iterator usedRU = std::find(renderUnitsUsed.begin(), renderUnitsUsed.end(), ruSharedMaterial->id());
       		if (usedRU == renderUnitsUsed.end())
       		{
       			ru->m_modelMatricesBuffer->addBatchData({ modelMatrix });
       			renderUnitsUsed.push_back(ruSharedMaterial->id());
       		}
       	}
	}

	void ModelRenderer::setModel(const std::string& relativePath)
   	{
  		m_meshPath = relativePath;
  		LoadModel();
   	}
    void ModelRenderer::LoadModel()
   	{
   	    if (!m_meshPath.empty() && m_model.lock())
   		{
   			ResourceManager::UnloadModel(m_meshPath, m_shaderProgramType);

   			// for (auto& [matId, ruPtr] : m_cachedRenderUnits)
   			// {
   			// 	Renderer::DestroyRenderUnit(ruPtr.lock(), ruPtr.lock()->m_drawingData.lock());
   			// }
   			m_cachedRenderUnits.clear();
   		}
   		m_model = ResourceManager::LoadModel(m_meshPath, m_shaderProgramType);

   		for (const MeshSPtr& mesh : m_model.lock()->m_meshes)
   		{
   			RenderUnitSPtr ru = Renderer::GetRenderUnit(mesh->m_material, m_shaderProgramType);
   			if (ru == nullptr)
   			{
   				ru = Renderer::CreateRenderUnit(m_shaderProgramType, mesh->m_material);
   			}

   			m_cachedRenderUnits[mesh->m_material.lock()->id()] = ru;
   			auto meshId = std::find(ru->m_meshesRegistered.begin(), ru->m_meshesRegistered.end(), mesh->id());
   			if (meshId != ru->m_meshesRegistered.end()) continue;
   			ru->addBatchData(mesh->m_vertices, mesh->m_indices, {});
   			ru->m_staticStorage = true;
   			ru->m_meshesRegistered.push_back(mesh->id());
   		}
   	}

    REGISTER_COMPONENT(ModelRenderer);
}
