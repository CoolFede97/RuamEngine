#pragma once

#include "Component.hpp"
#include "Renderer.h"
#include "Vertex.h"
#include "SSBO.h"

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

	unsigned int indexCount = 0;

	// It's called in update
	void render()
	{
		RenderUnit& genericUnit = Renderer::m_drawingDataMap[Shader::PipelineType::Generic]->m_renderUnits[Material::MaterialType::Generic];
		//genericUnit.m_vertexArray->Bind();

		//genericUnit.m_shader->Bind();
		
		indexCount = 0;
		for (int row = 0; row < gridSide; row++)
		{
			for (int col = 0; col < gridSide; col++)
			{				
				std::vector<Vertex> newQuad = Vertex::CreateQuad
				(
					quadWidth,
					col * (screenX / gridSide - 0.5f * padding) + col * padding + quadWidth / 2 - screenX / 2,
					row * (screenY / gridSide - 0.5f * padding) + row * padding + quadHeight / 2 - screenY / 2,
					1.0f
				);
				
				std::vector<unsigned int> newIndices =
				{
					indexCount + 0, indexCount + 1, indexCount + 2,indexCount + 2, indexCount + 3, indexCount + 0
				};
				indexCount += 4;
				if (genericUnit.AddBatchData(newQuad, newIndices, { glm::mat4(1.0f) }))
				{
					indexCount = 0;
				}
				
			}
		}
	};
	void start() 
	{
		auto quad = Vertex::CreateQuad(0, 0);
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		Renderer::m_state.m_vertexArray->Bind();
		Renderer::m_state.m_vertexBuffer->SetSubData(0, sizeof(quad), &quad);
		Renderer::m_state.m_layout->Reset();
		Renderer::m_state.m_layout->Push<float>(3);
		Renderer::m_state.m_layout->Push<float>(4);

		// PREGUNTAR A TOMI CUANDO VEAS ESTO TOOFEOKGFEOGKEROGKRE FFOKE
		Renderer::m_state.m_indexBuffer->SetSubData(0, sizeof(indices), &indices[0]);
		Renderer::m_state.m_vertexArray->AddBuffer(*Renderer::m_state.m_vertexBuffer, *Renderer::m_state.m_layout);
		
		Renderer::m_state.m_shader = std::make_shared<Shader>("assets/shaders/GeneralVertexShader.glsl", "assets/shaders/GeneralFragmentShader.glsl");
		Renderer::m_state.m_shader->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		Renderer::m_state.m_shader->SetUniformMat4f("u_model", model);
		Renderer::m_state.m_shader->SetUniformMat4f("u_view", model);
		Renderer::m_state.m_shader->SetUniformMat4f("u_projection", model);

	};
};
