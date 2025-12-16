#pragma once

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

	using BaseRenderer::BaseRenderer;
	MeshRenderer(const nlohmann::json& j, unsigned int obj_id) : BaseRenderer(obj_id)
	{
		if (j.contains("m_meshPath"))
		{
			SetModel(j["m_meshPath"]);
		}
	}
    // IMPL_SIMPLE_SERIALIZE(MeshRenderer)
	void SetModel(const std::string& path)
	{
		m_meshPath = path;
		m_model = std::make_shared<Model>(m_meshPath);
		m_vertices = GetMeshesVertices();
		m_indices = GetMeshesIndices();
		Renderer::UpdateTextureType(GL_TEXTURE_2D);

		// Pre-upload geometry once per mesh into the appropriate RenderUnit (vertices + indices).
		for (Mesh& mesh : m_model->m_meshes)
		{
			for (auto& ru : Renderer::m_drawingDatas[0]->m_renderUnits)
			{
				if (ru->m_material->GetId() == mesh.m_material->GetId())
				{
					ru->AddBatchData(mesh.m_vertices, mesh.m_indices, {});
					ru->m_staticStorage = true;
					break;
				}
			}
		}
	}
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

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
		/*for (unsigned int i = 0; i < m_indices.size(); i++)
		{
			std::vector<Vertex> localVertices;
			if (i<m_vertices.size())
			{
				localVertices = { m_vertices[i] };
			}
			else localVertices = {};
			genericUnit.AddBatchData(localVertices, { m_indices[i] }, {});
		}*/

		//genericUnit.AddBatchData(m_vertices, m_indices, { modelMatrix });
	};

	std::vector<Vertex> GetMeshesVertices()
	{
		std::vector<Vertex> allVertices;
		for (const Mesh& mesh : m_model->m_meshes)
		{
			allVertices.insert(allVertices.end(), mesh.m_vertices.begin(), mesh.m_vertices.end());
		}
		return allVertices;
	}

	std::vector<unsigned int> GetMeshesIndices()
	{
		std::vector<unsigned int> allIndices;
		for (const Mesh& mesh : m_model->m_meshes)
		{
			for (unsigned int index : mesh.m_indices)
			{
				allIndices.push_back(index);
			}
		}
		return allIndices;
	}

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
