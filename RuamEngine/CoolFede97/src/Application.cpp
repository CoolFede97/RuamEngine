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
#include "RuamEngine.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <filesystem>

#include "Vec3.h"

#include "imgui.h"

#include "Input.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestMovement.h"
#include "tests/Sandbox.h"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

using namespace RuamEngine;

int main(void)
{
	Renderer::Init();

	{
		Input::SetWindow(Renderer::GetWindow());
		ImGui::CreateContext();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplGlfw_InitForOpenGL(Renderer::GetWindow(), true);
		ImGui_ImplOpenGL3_Init();
		ImGui::StyleColorsDark();

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestMovement>("Movement Test");
		testMenu->RegisterTest<test::Sandbox>("Sandbox");

		while (!glfwWindowShouldClose(Renderer::GetWindow()))
		{
			// ImGUI
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Input
			Input::UpdateInput();
			
			// Time
			RuamEngine::Time::Update();

			Renderer::BeginDraw();
			
			if (currentTest)
			{
				currentTest->Update();
				currentTest->Render();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->ImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			Renderer::EndDraw();

			glfwPollEvents();

		}
		delete currentTest;
		if (currentTest != testMenu)
			delete testMenu;

	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	ImGui::DestroyContext();
	Renderer::Shutdown();
	return 0;
}
