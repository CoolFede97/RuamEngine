#include "Engine.h"
#include "GameCamera.h"
#include "KeyCode.h"
#include "Renderer.h"
#include "Input.h"
#include "RenderingCore.h"
#include "RuamTime.h"
#include "Editor.h"
#include "EventManager.h"
#include "SaveSystem.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Serial.h"
#include "ComponentsInitializer.h"
#include "EditorCamera.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace RuamEngine
{
    bool Engine::s_initialized = false;
    bool Engine::s_started = false;
    RuamConfig Engine::s_config;
    EngineState Engine::s_state = EngineState::EditorMode;

    void Engine::Init()
    {
        if (s_initialized)
        {
            std::cerr << "Can't call Engine::SetUp() because the engine is already set up\n";
            return;
        }
        Renderer::Init();
        ComponentsInitializer::InitComponents();
       	// AudioSystem::init();

  		Input::SetWindow(Renderer::GetWindow());
  		Input::SetUp(Renderer::GetWindow());
  		IMGUI_CHECKVERSION();
  		ImGui::CreateContext();
  		ImGuiIO& io = ImGui::GetIO();
        ImFont* regularFont = io.Fonts->AddFontFromFileTTF((std::string(PROJECT_ROOT_DIR)+"/"+"RuamCore/Fonts/AdwaitaMonoNerdFont-Regular.ttf").c_str());
        ImFont* BoldFont = io.Fonts->AddFontFromFileTTF((std::string(PROJECT_ROOT_DIR)+"/"+"RuamCore/Fonts/AdwaitaMonoNerdFont-Bold.ttf").c_str());
        ImFont* largerRegularFont = io.Fonts->AddFontFromFileTTF((std::string(PROJECT_ROOT_DIR)+"/"+"RuamCore/Fonts/AdwaitaMonoNerdFont-Regular.ttf").c_str(), 24.0f);
        ImFont* largerBoldFont = io.Fonts->AddFontFromFileTTF((std::string(PROJECT_ROOT_DIR)+"/"+"RuamCore/Fonts/AdwaitaMonoNerdFont-Bold.ttf").c_str(), 24.0f);
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  		ImGui_ImplGlfw_InitForOpenGL(Renderer::GetWindow(), true);
  		ImGui_ImplOpenGL3_Init();

  		ImGui::StyleColorsDark();

   		// LoadRuamConfig();
    	SceneManager::UpdateScenes();
        s_initialized = true;
    }

    void Engine::Start()
    {
        if (s_started)
        {
            std::cerr << "Can't call Engine::Init() because the engine is already initialized\n";
            return;
        }
        s_started = true;
       	unsigned int frameCount = 0;
        if (SceneManager::Scenes().size()>0) SceneManager::EnqueueSceneChange(SceneManager::Scenes()[0], true);

  		while (!Renderer::WindowShouldClose())
  		{
    		CheckIfWantToSaveChanges();
 			SceneManager::CheckForSceneChange();
  		    // std::cout << "Frame count: " << frameCount++ << "\n";

            ImGuiStartNewFrame();

            Editor::UpdateHierarchy();
            Editor::UpdateInspector();
            Editor::UpdateSceneManager();
            Editor::UpdateViewport(Renderer::s_editorFrameBuffer.get(), "Editor", true);
            Editor::UpdateViewport(Renderer::s_gameFrameBuffer.get(), "Game", false);
            Editor::UpdateCameraTransform();
            // Time
 			RuamTime::Update();

 			EventManager::HandleEvents();

            Scene* scene = SceneManager::ActiveScene();
            // std::cout << "Frame count: " << frameCount++ << "\n";
 			if (scene)
 			{
                if (Input::GetKey(KeyCode::LeftControl_Key) && Input::GetKeyDown(KeyCode::P_Key))
                {
                    if (s_state == EngineState::EditorMode)
                    {
                        s_state = EngineState::GameMode;
                        std::cout << "Entered into Game Mode\n";
                    }
                    else
                    {
                        SceneManager::EnqueueSceneChange(scene->name(), false);
                        s_state = EngineState::EditorMode;
                        std::cout << "Entered into Editor Mode\n";
                    }
                }
				scene->tick();
 			}

            Renderer::s_editorFrameBuffer->bind();
 			Renderer::ClearScreen();
            Renderer::s_gameFrameBuffer->bind();
 			Renderer::ClearScreen();
            Renderer::s_gameFrameBuffer->unbind();

            if (!SceneManager::SceneChange() && scene)
 			{
                Renderer::s_editorFrameBuffer->bind();
     			Renderer::ClearScreen();
          		Renderer::Draw(Editor::Camera().viewMatrix(), Editor::Camera().projectionMatrix());
                Renderer::s_editorFrameBuffer->unbind();

                if (GameCamera::GetMainCamera())
                {
                    Renderer::s_gameFrameBuffer->bind();
                    Renderer::ClearScreen();
                   	Renderer::Draw(GameCamera::GetMainCamera()->viewMatrix(), GameCamera::GetMainCamera()->projectionMatrix());
                    Renderer::s_gameFrameBuffer->unbind();
                }
            }

            GLCall(glViewport(0,0,Renderer::GetWindowWidth(), Renderer::GetWindowHeight()));
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            Renderer::ClearScreen();
            ImGui::Render();
           	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            Renderer::EndDraw();
            Input::UpdateInput();
            glfwPollEvents();
            if (scene) scene->checkForEntitiesDestruction();
            SceneManager::CheckForSceneDeletion();
    	}
    	// Cleanup
        SceneManager::s_activeScene = nullptr;
    	ImGui_ImplOpenGL3_Shutdown();
    	ImGui_ImplGlfw_Shutdown();
    	ImGui::DestroyContext();
    	Renderer::Shutdown();
    	// AudioSystem::shutdown();
    }

    void Engine::CheckIfWantToSaveChanges()
    {
    	if (Input::GetKey(KeyCode::LeftControl_Key) && Input::GetKeyDown(KeyCode::S_Key))
     	{
            if (s_state == EngineState::EditorMode) SaveSystem::SaveCurrentScene();
            else std::cerr << "Can't save scene during game mode!\n"; // TODO UI
     	}
    }

    void Engine::ImGuiStartNewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Engine::LoadRuamConfig()
    {
   		s_config = Serial::DeserializeRuamConfig(SaveSystem::LoadJsonRuamConfig());
    }
}
