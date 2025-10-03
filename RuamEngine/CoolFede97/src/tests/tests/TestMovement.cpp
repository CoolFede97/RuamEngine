#include "TestMovement.h"

#include "Test.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "Renderer.h"
#include "Input.h"
#include "RuamTime.h"
namespace test
{
	//TestMovement::TestMovement()
	//{
	//	Start();
	//}

	//TestMovement::~TestMovement()
	//{
	//	delete[] m_CubeLocalVertices;
	//	delete[] m_CubeGlobalPositions;
	//	delete m_VAO;
	//	delete m_VBO;
	//	delete m_Layout;
	//	delete m_Shader;
	//	delete m_Camera;
	//}

	//void TestMovement::Start()
	//{
	//	GLCall(glClearColor(0.24f, 0.24f, 0.16f, 1.0f));


	//	float vertices[] = {
	//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	// 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	// 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	// 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	//-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	//	};
	//	m_CubeLocalVertices = new float[180]
	//	{
	//		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	//		 0.5f,  -0.5f,  -0.5f,  1.0f,  0.0f,
	//		 0.5f,  0.5f,  -0.5f,  1.0f,  1.0f,
	//		 0.5f,  0.5f,  -0.5f,  1.0f,  1.0f,
	//		 -0.5f,  0.5f,  -0.5f,  0.0f,  1.0f,
	//		 -0.5f,  -0.5f,  -0.5f,  0.0f,  0.0f,

	//		 -0.5f,  -0.5f,  0.5f,  0.0f,  0.0f,
	//		 0.5f,  -0.5f,  0.5f,  1.0f,  0.0f,
	//		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
	//		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
	//		 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
	//		 -0.5f,  -0.5f,  0.5f,  0.0f,  0.0f,

	//		 -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
	//		 -0.5f,  0.5f,  -0.5f,  1.0f,  1.0f,
	//		 -0.5f,  -0.5f,  -0.5f,  0.0f,  1.0f,
	//		 -0.5f,  -0.5f,  -0.5f,  0.0f,  1.0f,
	//		 -0.5f,  -0.5f,  0.5f,  0.0f,  0.0f,
	//		 -0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

	//		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
	//		 0.5f,  0.5f,  -0.5f,  1.0f,  1.0f,
	//		 0.5f,  -0.5f,  -0.5f,  0.0f,  1.0f,
	//		 0.5f,  -0.5f,  -0.5f,  0.0f,  1.0f,
	//		 0.5f,  -0.5f,  0.5f,  0.0f,  0.0f,
	//		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

	//		 -0.5f,  -0.5f,  -0.5f,  0.0f,  1.0f,
	//		 0.5f,  -0.5f,  -0.5f,  1.0f,  1.0f,
	//		 0.5f,  -0.5f,  0.5f,  1.0f,  0.0f,
	//		 0.5f,  -0.5f,  0.5f,  1.0f,  0.0f,
	//		 -0.5f,  -0.5f,  0.5f,  0.0f,  0.0f,
	//		 -0.5f,  -0.5f,  -0.5f,  0.0f,  1.0f,

	//		 -0.5f,  0.5f,  -0.5f,  0.0f,  1.0f,
	//		 0.5f,  0.5f,  -0.5f,  1.0f,  1.0f,
	//		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
	//		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
	//		 -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
	//		 -0.5f,  0.5f,  -0.5f,  0.0f,  1.0f
	//	};

	//	m_CubeGlobalPositions = new glm::vec3[20]
	//	{
	//		{0.0f,  10.0f,  -15.0f},
	//		{2.0f,  5.0f, -15.0f},
	//		{-1.5f, -2.2f, -2.5f},
	//		{-3.8f, -2.0f, -12.3f},
	//		{2.4f, -0.4f, -3.5f},
	//		{-1.7f,  3.0f, -7.5f},
	//		{1.3f, -2.0f, -2.5f},
	//		{1.5f,  2.0f, -2.5f},
	//		{1.5f,  0.2f, -1.5f},
	//		{-1.3f,  1.0f, -1.5f},
	//		{0.0f,   0.0f,   -10.0f},
	//		{-2.0f,  -5.0f,  15.0f},
	//		{1.5f,   2.2f,   2.5f},
	//		{3.8f,   2.0f,  12.3f},
	//		{-2.4f,   0.4f,   3.5f},
	//		{1.7f,  -3.0f,   7.5f},
	//		{-1.3f,   2.0f,   2.5f},
	//		{-1.5f,  -2.0f,   2.5f},
	//		{-1.5f,  -0.2f,   1.5f},
	//		{1.3f,  -1.0f,   1.5f}

	//	};
	//	m_CubeGlobalPositionsCount = 20;

	//	m_VAO = new VertexArray();
	//	m_VBO = new VertexBuffer(vertices, 5 * 36 * sizeof(float));
	//	m_Layout = new VertexBufferLayout();
	//	m_Layout->Push<float>(3); // Position
	//	m_Layout->Push<float>(2); // Texture Coordinates
	//	m_VAO->AddBuffer(*m_VBO, *m_Layout);
	//
	//	m_Shader = new Shader("CoolFede97/res/shaders/camera/CameraVertex.txt", "CoolFede97/res/shaders/camera/CameraFragment.txt");
	//	m_Shader->Bind();

	//	m_Shader->Unbind();
	//	m_Camera = new Camera();
	//	m_CameraSpeed = 2.0f;
	//}

	//void TestMovement::Update()
	//{
	//	MoveCamera();
	//}

	//void TestMovement::Render()
	//{
	//	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//	m_Shader->Bind();

	//	m_Shader->SetUniformMat4f("u_view", m_Camera->GetViewMatrix());
	//	m_Shader->SetUniformMat4f("u_projection", m_Camera->GetProjectionMatrix());

	//	for (int i = 0; i < m_CubeGlobalPositionsCount; i++)
	//	{
	//		glm::mat4 model = glm::mat4(1.0f);
	//		model = glm::translate(model, m_CubeGlobalPositions[i]);
	//		m_Shader->SetUniformMat4f("u_model", model);
	//		m_VAO->Bind();
	//		glDrawArrays(GL_TRIANGLES, 0, 36);
	//	}
	//}

	//void TestMovement::ImGuiRender()
	//{
	//	ImGui::SliderFloat("Camera Speed", &m_CameraSpeed, 2.0, 10.0f);
	//}

	//void TestMovement::MoveCamera()
	//{
	//	glm::vec3 camera_dir = m_Camera->m_direction;
	//	glm::vec3 camera_up = m_Camera->m_up;
	//	if (glm::length(glm::cross(camera_dir, camera_up)) == 0.0f)
	//		std::cout << "Error: camera_dir and camera_up can't be parallel (because it results in a NaN when normalizing 0)" << "\n";
	//	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	//	if (glfwGetKey(Input::GetWindow(), GLFW_KEY_D) == GLFW_PRESS)
	//	{
	//		direction += glm::normalize(glm::cross(camera_dir, camera_up));
	//	}
	//	if (glfwGetKey(Input::GetWindow(), GLFW_KEY_A) == GLFW_PRESS)
	//	{
	//		direction -= glm::normalize(glm::cross(camera_dir, camera_up));
	//	}
	//	if (glfwGetKey(Input::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	//	{
	//		direction += camera_dir;
	//	}
	//	if (glfwGetKey(Input::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	//	{
	//		direction -= camera_dir;
	//	}
	//	m_Camera->m_position += direction * m_CameraSpeed * ruamTime::Time::DeltaTime();
	//}

}
