#include "TestTextures.h"

#include "Test.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "Renderer.h"
#include "Input.h"
namespace test
{
	//TestTextures::TestTextures()
	//{
	//	Start();
	//}

	//TestTextures::~TestTextures()
	//{
	//	delete[] m_Vertices;
	//	delete m_VAO;
	//	delete m_VBO;
	//	delete m_Layout;
	//	delete m_Shader;
	//	delete m_Texture;
	//}

	//void TestTextures::Start()
	//{
	//	m_VAO = new VertexArray();
	//	m_VBO = new VertexBuffer(m_Vertices, 5 * 36 * sizeof(float));
	//	m_Layout = new VertexBufferLayout();
	//	m_Layout->Push<float>(3); // Position
	//	m_Layout->Push<float>(2); // Texture Coordinates
	//	m_VAO->AddBuffer(*m_VBO, *m_Layout);
	//
	//	m_Shader = new Shader("CoolFede97/res/shaders/BasicVertex.glsl", "CoolFede97/res/shaders/BasicFragment.glsl");
	//	m_Shader->Bind();

	//	m_Texture = new Texture("CoolFede97/res/textures/francoKO.jpg");
	//	m_Texture->Bind();

	//	m_Shader->SetUniform1i("u_Texture", 0);
	//	m_Shader->Unbind();
	//}

	//void TestTextures::Update()
	//{
	//}

	//void TestTextures::Render()
	//{
	//	GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
	//	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//	m_Shader->Bind();

	//}

	//void TestTextures::ImGuiRender()
	//{
	//}
}
