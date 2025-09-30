#pragma once

#include <Component.hpp>
#include "Renderer.h"
#include "Vertex.h"
#include "VertexBuffer.h"


using namespace RuamEngine;

class SandboxCom2 : public BaseRenderer
{
	using BaseRenderer::BaseRenderer;

	// It's called in update
	void render()
	{
		std::cout << "Render from component called\n";
		RenderUnit& genericUnit = Renderer::m_drawingDataMap[Shader::PipelineType::Generic]->m_renderUnits[Material::MaterialType::Generic];
		genericUnit.m_vertexArray->Bind();

		int gridSide = 2; // k*k grid
		float padding = 0.1f;
		float screenX = 2.0f;
		float screenY = 2.0f;

		float quadWidth = screenX / gridSide - padding / 2;
		float quadHeight = screenY / gridSide - padding / 2;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		/*vertices.reserve(50000 * 4);
		indices.reserve(50000 * 6);*/

		

		genericUnit.m_shader->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		genericUnit.m_shader->SetUniformMat4f("u_model", model);
		genericUnit.m_shader->SetUniformMat4f("u_view", model);
		genericUnit.m_shader->SetUniformMat4f("u_projection", model);

		for (int row = 0; row < gridSide; ++row)
		{
			for (int col = 0; col < gridSide; ++col)
			{
				auto newQuad = Vertex::CreateQuad
				(
					quadWidth,
					col * (screenX / gridSide - 0.5f * padding) + col * padding + quadWidth / 2 - screenX / 2,
					row * (screenY / gridSide - 0.5f * padding) + row * padding + quadHeight / 2 - screenY / 2
				);
				vertices.insert(vertices.end(), newQuad.begin(), newQuad.end());

				genericUnit.m_vertexBuffer->SetData(vertices.data());

				genericUnit.m_layout->Reset();
				genericUnit.m_layout->Push<float>(3);
				genericUnit.m_layout->Push<float>(4);

				indices =
				{
					0, 1, 2,
					2, 3, 0
				};

				genericUnit.m_indexBuffer->SetData(indices.data());

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
				Renderer::Flush();
				vertices.clear();
			}
		}
	};
	void update()
	{
		render();
	};

};
