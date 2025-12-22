#pragma once

#include "RenderUnit.h"
#include "ResourceManager.h"
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
using namespace RuamEngine;

class MeshRenderer : public BaseRenderer
{
public:
	std::string m_meshPath;
	ModelPtr m_model;
	ShaderProgramType m_shaderProgramType;
	RenderUnitPtr m_renderUnit;

	using BaseRenderer::BaseRenderer;
	MeshRenderer(const nlohmann::json& j, unsigned int obj_id) : BaseRenderer(obj_id)
	{
		if (j.contains("m_meshPath"))
		{
			SetModel(j["m_meshPath"]);
		}
	}

	~MeshRenderer()
	{
		if (!m_meshPath.empty())
		{
			ResourceManager::UnloadModel(m_meshPath, m_shaderProgramType);
			m_myRenderUnits.clear();
		}
	}

    // IMPL_SIMPLE_SERIALIZE(MeshRenderer)
	void SetModel(const std::string& relativePath)
	{
	    if (!m_meshPath.empty() && m_model!=nullptr)
		{
			ResourceManager::UnloadModel(relativePath, m_shaderProgramType);

			for (auto& ruPtr : m_myRenderUnits)
			{
				Renderer::DestroyRenderUnit(ruPtr, ruPtr->m_drawingData);
			}
			m_myRenderUnits.clear();
		}

		m_meshPath = relativePath;
		m_model = ResourceManager::LoadModel(m_meshPath, m_shaderProgramType);
		Renderer::UpdateTextureType(GL_TEXTURE_2D);

		// Pre-upload geometry once per mesh into the appropriate RenderUnit (vertices + indices).
		for (Mesh& mesh : m_model->m_meshes)
		{
			for (auto& ru : Renderer::m_drawingDatas[m_shaderProgramType]->m_renderUnits)
			{
				if (ru->m_material->GetId() == mesh.m_material->GetId())
				{
					if (ru->m_indices->GetCurrentSize()>0) return; // If the current size is bigger than 0, it means that the model data is already loaded
					ru->AddBatchData(mesh.m_vertices, mesh.m_indices, {});
					ru->m_staticStorage = true;
					m_myRenderUnits.push_back(ru);
					break;
				}
			}
		}
	}
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<RenderUnitPtr> m_myRenderUnits;

	void render()
	{
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, object()->transform().position());
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, object()->transform().scale());

        // bool test = m_model == nullptr;
        // if (test) std::cout << "Model is null\n";
        // else std::cout << "Model exists\n";
        //

        if (m_model->m_localToGlobalMaterials.size() == 1)
		{
			Mesh mesh = m_model->m_meshes[0];
			//for (Mesh mesh : m_model->m_meshes)
			//{
				for (auto& ru : Renderer::m_drawingDatas[0]->m_renderUnits)
				{
					if (ru->m_material->GetId() == mesh.m_material->GetId())
					{
						Renderer::matrices.push_back(modelMatrix);
						ru->m_modelMatricesBuffer->AddBatchData({ modelMatrix });
						return;
					}
				}
			//}
		}

		std::vector<unsigned int> renderUnitsUsed = {};
		for (Mesh mesh : m_model->m_meshes)
		{
			for (auto& ru : Renderer::m_drawingDatas[0]->m_renderUnits)
			{
				if (ru->m_material->GetId() == mesh.m_material->GetId())
				{
					std::vector<unsigned int>::iterator it = std::find(renderUnitsUsed.begin(), renderUnitsUsed.end(), ru->m_material->GetId());
					if (it == renderUnitsUsed.end())
					{
						ru->m_modelMatricesBuffer->AddBatchData({ modelMatrix });
						renderUnitsUsed.push_back(ru->m_material->GetId());
					}
					break;
				}
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
	IMPL_SERIALIZE(MeshRenderer,
	SER_FIELD(m_meshPath));
};
	REGISTER_COMPONENT(MeshRenderer)
