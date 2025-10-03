#pragma once

#include "Component.hpp"
#include "Renderer.h"
#include "Vertex.h"
#include "VertexBuffer.h"

using namespace RuamEngine;

class SandboxCom : public BaseRenderer
{
	using BaseRenderer::BaseRenderer;
	
	int gridSide = 1; // k*k grid
	float padding = 0.01f;
	float screenX = 2.0f;
	float screenY = 2.0f;

	float quadWidth = screenX / gridSide - padding / 2;
	float quadHeight = screenY / gridSide - padding / 2;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int indexCount = 0;

	// It's called in update
	void render()
	{
		RenderUnit& genericUnit = Renderer::m_drawingDataMap[Shader::PipelineType::Generic]->m_renderUnits[Material::MaterialType::Generic];
		genericUnit.m_vertexArray->Bind();

		vertices.reserve(gridSide * gridSide * 4);
		indices.reserve(gridSide * gridSide * 6);


		genericUnit.m_shader->Bind();
		

		for (int row = 0; row < gridSide; row++)
		{
			for (int col = 0; col < gridSide; col++)
			{				
				std::vector<float> newQuad = VertexWithTex::FlattenVertices(VertexWithTex::CreateQuad
				(
					quadWidth,
					col * (screenX / gridSide - 0.5f * padding) + col * padding + quadWidth / 2 - screenX / 2,
					row * (screenY / gridSide - 0.5f * padding) + row * padding + quadHeight / 2 - screenY / 2,
					1
				));
				vertices.insert(vertices.end(), newQuad.begin(), newQuad.end());
				
				std::vector<unsigned int> newIndices =
				{
					indexCount + 0, indexCount + 1, indexCount + 2,indexCount + 2, indexCount + 3, indexCount + 0
				};

				indices.insert(indices.end(), newIndices.begin(), newIndices.end());

				indexCount += 4;

				if (sizeof(float) * VertexWithTex::attributesFloatCount * indexCount + sizeof(float) * VertexWithTex::attributesFloatCount * 4 >= genericUnit.m_vertexBuffer->GetMaxSize() - genericUnit.m_vertexBuffer->GetCurrentSize()
					||
					sizeof(unsigned int) * indices.size() + sizeof(unsigned int) * 6 >= genericUnit.m_indexBuffer->GetMaxSize() - genericUnit.m_indexBuffer->GetCurrentSize()
					)
				{
					if (genericUnit.AddBatchData(vertices, vertices.size() * sizeof(float), indices, indices.size() * sizeof(unsigned int)))
					{
						vertices.clear();
						indices.clear();
						indexCount = 0;
					}
				}
			}
		}
		if (indices.size() > 0)
		{
			genericUnit.AddBatchData(vertices, vertices.size() * sizeof(float), indices, indices.size() * sizeof(unsigned int));
			vertices.clear();
			indices.clear();
			indexCount = 0;
		}
	};
	void update() 
	{
		render();
	};

};
