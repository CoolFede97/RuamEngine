#pragma once

#include "Component.hpp"
#include "Renderer.h"
#include "Vertex.h"
#include "VertexBuffer.h"

using namespace RuamEngine;

class SandboxCom : public BaseRenderer
{
	using BaseRenderer::BaseRenderer;
	
	// It's called in update
	void render()
	{
		Renderer::Draw();
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
