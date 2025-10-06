#include <iostream>
#include <assert.h>

#include "Renderer.h"

#include "SSBO.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h" 
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "RuamTime.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <filesystem>

#include "Vec3.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "Input.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestMovement.h"
#include "tests/Sandbox.h"

using namespace RuamEngine;

int main(void)
{
	//Renderer::Init();

	//{
	//	Input::SetWindow(Renderer::GetWindow());
	//	ImGui::CreateContext();
	//	ImGui_ImplGlfwGL3_Init(Renderer::GetWindow(), true);
	//	ImGui::StyleColorsDark();

	//	test::Test* currentTest = nullptr;
	//	test::TestMenu* testMenu = new test::TestMenu(currentTest);
	//	currentTest = testMenu;

	//	testMenu->RegisterTest<test::TestClearColor>("Clear Color");
	//	testMenu->RegisterTest<test::TestMovement>("Movement Test");
	//	testMenu->RegisterTest<test::Sandbox>("Sandbox");

	//	while (!glfwWindowShouldClose(Renderer::GetWindow()))
	//	{
	//		// ImGUI
	//		ImGui_ImplGlfwGL3_NewFrame();

	//		// Input
	//		Input::UpdateInput();
	//		
	//		// Time
	//		ruamTime::Time::Update();

	//		if (currentTest)
	//		{
	//			currentTest->Update();
	//			currentTest->Render();
	//			ImGui::Begin("Test");
	//			if (currentTest != testMenu && ImGui::Button("<-"))
	//			{
	//				delete currentTest;
	//				currentTest = testMenu;
	//			}
	//			currentTest->ImGuiRender();
	//			ImGui::End();
	//		}

	//		ImGui::Render();
	//		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

	//		Renderer::EndDraw();

	//		glfwPollEvents();

	//	}
	//	delete currentTest;
	//	if (currentTest != testMenu)
	//		delete testMenu;

	//}
	//// Cleanup
	//ImGui_ImplGlfwGL3_Shutdown();
	//ImGui::DestroyContext();
	//Renderer::Shutdown();
	//return 0;
}
