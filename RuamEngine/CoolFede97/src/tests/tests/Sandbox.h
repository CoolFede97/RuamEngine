#pragma once

#include "Test.h"
#include "Renderer.h"
#include "Texture.h"
#include <Vertex.h>
#include "VertexBufferLayout.h"

namespace test
{
	class Sandbox : public Test
	{
	public:
		Sandbox();
		~Sandbox();
		
		void Start() override;	
		void Update() override;
		void Render() override;
		void ImGuiRender() override;
	private:
		VertexArray* m_VAO;
		VertexBuffer* m_VBO;
		IndexBuffer* m_IBO;
		VertexBufferLayout* m_Layout;
		Shader* m_Shader;
		Texture* m_Texture;
	};
}
