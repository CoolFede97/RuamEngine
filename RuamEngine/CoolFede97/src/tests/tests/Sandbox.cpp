#include "Sandbox.h"

#include "Renderer.h"
#include "imgui.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
namespace test
{

	//Sandbox::Sandbox()
	//{
	//	Start();
	//}

	//Sandbox::~Sandbox()
	//{

	//}

	//void Sandbox::Start()
	//{
	//	GLCall(glClearColor(0.24f, 0.24f, 0.16f, 1.0f));

	//	auto quad = VertexWithTex::CreateQuad(0.0f, 0.0f);

	//	m_VAO = new VertexArray();
	//	m_VBO = new VertexBuffer(&quad, sizeof(float) * quad.size() * 6);
	//	
	//	m_Layout = new VertexBufferLayout();
	//	m_Layout->Push<float>(3); // Position
	//	m_Layout->Push<float>(2); // Texture Coordinates
	//	m_Layout->Push<float>(1); // Texture ID
	//	m_VAO->AddBuffer(*m_VBO, *m_Layout);

	//	unsigned int indices[] =
	//	{
	//		0, 1, 2,
	//		2, 3, 0
	//	};

	//	m_IBO = new IndexBuffer(indices, 6);

	//	m_Shader = new Shader("CoolFede97/res/shaders/basic/BasicVertex.txt", "CoolFede97/res/shaders/basic/BasicFragment.txt");
	//	m_Shader->Bind();


	//}

	//void Sandbox::Update()
	//{

	//}

	//void Sandbox::Render()
	//{
	//	m_Shader->Bind();
	//	m_VAO->Bind();
	//	//glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
	//}

	//void Sandbox::ImGuiRender()
	//{
	//}

}
