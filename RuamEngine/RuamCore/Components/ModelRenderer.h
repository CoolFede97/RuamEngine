#pragma once

#include "RenderUnit.h"
#include "ResourceManager.h"
#include "RuamUtils.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"
#include "Renderer.h"
#include "Vertex.h"
#include <string>
#include <vector>
#include "Model.h"
#include "Transform.h"

using namespace RuamEngine;

class ModelRenderer : public Component
{
public:
	std::string m_meshPath;
	ModelWPtr m_model;
	ShaderProgramType m_shaderProgramType;
	std::unordered_map<unsigned int, RenderUnitWPtr> m_cachedRenderUnits;

	using Component::Component;

	~ModelRenderer()
	{
		if (!m_meshPath.empty())
		{
			ResourceManager::UnloadModel(m_meshPath, m_shaderProgramType);
			m_cachedRenderUnits.clear();
		}
	}

	void setModel(const std::string& relativePath)
	{
	    if (!m_meshPath.empty() && m_model.lock())
		{
			ResourceManager::UnloadModel(relativePath, m_shaderProgramType);

			for (auto& [matId, ruPtr] : m_cachedRenderUnits)
			{
				Renderer::DestroyRenderUnit(ruPtr.lock(), ruPtr.lock()->m_drawingData.lock());
			}
			m_cachedRenderUnits.clear();
		}

		m_meshPath = relativePath;
		m_model = ResourceManager::LoadModel(m_meshPath, m_shaderProgramType);

		// Pre-upload geometry once per mesh into the appropriate RenderUnit (vertices + indices).
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
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	// std::vector<RenderUnitSPtr> m_myRenderUnits;

	void start()
	{
	}


	void update()
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
    			// Renderer::matrices.push_back(modelMatrix);
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
};
