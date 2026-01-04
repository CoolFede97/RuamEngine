#pragma once

#include "RenderUnit.h"
#include "ResourceManager.h"
#include "RuamUtils.h"
#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"
#include <Component.hpp>
#include "Renderer.h"
#include "Vertex.h"
#include "RuamTime.h"
#include <string>
#include <vector>
#include "Model.h"
#include "FileFunctions.h"
#include <set>
using namespace RuamEngine;

class ModelRenderer : public BaseRenderer
{
public:
	std::string m_meshPath;
	ModelWPtr m_model;
	ShaderProgramType m_shaderProgramType;
	std::unordered_map<unsigned int, RenderUnitWPtr> m_cachedRenderUnits;

	using BaseRenderer::BaseRenderer;
	ModelRenderer(const nlohmann::json& j, unsigned int obj_id) : BaseRenderer(obj_id)
	{
		if (j.contains("m_meshPath"))
		{
			setModel(j["m_meshPath"]);
		}
	}

	~ModelRenderer()
	{
		if (!m_meshPath.empty())
		{
			ResourceManager::UnloadModel(m_meshPath, m_shaderProgramType);
			m_cachedRenderUnits.clear();
		}
	}

    // IMPL_SIMPLE_SERIALIZE(ModelRenderer)
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
		Renderer::UpdateTextureType(GL_TEXTURE_2D);

		// Pre-upload geometry once per mesh into the appropriate RenderUnit (vertices + indices).
		for (const MeshSPtr& mesh : m_model.lock()->m_meshes)
		{
			DrawingDataSPtr drawingData =  Renderer::m_drawingDatas[m_shaderProgramType];
			RenderUnitSPtr ru = Renderer::GetRenderUnit(mesh->m_material, drawingData);
			if (ru == nullptr)
			{
				ru = Renderer::CreateRenderUnit(drawingData, mesh->m_material);
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

	void render()
	{
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, object()->transform().position());
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, object()->transform().scale());

		ModelSPtr modelShared = GetShared(m_model);

        if (modelShared->m_localToGlobalMaterials.size() == 1)
		{
			MeshSPtr mesh = modelShared->m_meshes[0];
			RenderUnitSPtr ru = m_cachedRenderUnits[mesh->m_material.lock()->id()].lock();
			if (ru != nullptr)
			{
				Renderer::matrices.push_back(modelMatrix);
				ru->m_modelMatricesBuffer->addBatchData({ modelMatrix });
				return;
			}
		}

		std::vector<unsigned int> renderUnitsUsed = {};
		auto renderUnits = Renderer::m_drawingDatas[m_shaderProgramType]->m_renderUnits;
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
	};

	void start()
	{
	}


	void update()
	{
		BaseRenderer::update();
	};
	public:
	IMPL_SERIALIZE(ModelRenderer,
	SER_FIELD(m_meshPath));
};
	REGISTER_COMPONENT(ModelRenderer)
