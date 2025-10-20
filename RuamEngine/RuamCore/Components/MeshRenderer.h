#pragma once

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"
#include <Component.hpp>
#include "Renderer.h"
#include "Vertex.h"
#include "RuamTime.h"
#include <string>
#include "Model.h"
using namespace RuamEngine;

class MeshRenderer : public BaseRenderer
{
	using BaseRenderer::BaseRenderer;
public:
	std::string m_meshPath;
private:
	Model m_model;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	void render()
	{
		RenderUnit& genericUnit = Renderer::m_drawingDataMap[0]->m_renderUnits[0];

		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, object()->transform().position());
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, object()->transform().scale());

		
		genericUnit.AddBatchData(m_vertices, m_indices, { modelMatrix });
	};

	std::vector<Vertex> GetMeshesVertices()
	{
		std::vector<Vertex> allVertices;
		for (const Mesh& mesh : m_model.m_meshes)
		{
			allVertices.insert(allVertices.end(), mesh.m_vertices.begin(), mesh.m_vertices.end());
		}
		return allVertices;
	}

	std::vector<unsigned int> GetMeshesIndices()
	{
		std::vector<unsigned int> allIndices;
		for (const Mesh& mesh : m_model.m_meshes)
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
		m_model = Model(m_meshPath);
		m_vertices = GetMeshesVertices();
		m_indices = GetMeshesIndices();
	}

	void update()
	{
		BaseRenderer::update();
	};
};