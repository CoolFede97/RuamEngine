#include "Engine.h"
#include "KeyCode.h"
#include "Renderer.h"
#include "Input.h"
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
        if (SceneManager::Scenes().size()>0) SceneManager::EnqueueSceneChange(SceneManager::Scenes()[0]);


  		while (!Renderer::WindowShouldClose())
  		{
            // std::cout << "Mouse translation: " << Input::GetMouseDeltaPix() << "\n";
    		CheckIfWantToSaveChanges();
 			if (SceneManager::Scenes().size()>0) SceneManager::ApplyPendingSceneChange();
  		    // std::cout << "Frame count: " << frameCount++ << "\n";

 			// ImGUI
 			ImGui_ImplOpenGL3_NewFrame();
 			ImGui_ImplGlfw_NewFrame();
 			ImGui::NewFrame();

    		Editor::UpdateHierarchy();
            Editor::UpdateInspector();
            Editor::UpdateSceneManager();
            // Time
 			RuamTime::Update();

 			Renderer::ClearScreen();
 			Renderer::BeginBatch();

 			EventManager::HandleEvents();

            // std::cout << "Frame count: " << frameCount++ << "\n";
 			if (SceneManager::ActiveScene() != nullptr)
 			{
                if (Input::GetKey(KeyCode::LeftControl_Key) && Input::GetKeyDown(KeyCode::P_Key))
                {
                    if (s_state == EngineState::EditorMode) s_state = EngineState::GameMode;
                    else
                    {
                        SceneManager::EnqueueSceneChange(SceneManager::ActiveScene()->name());
                        s_state = EngineState::EditorMode;
                    }
                }
				SceneManager::ActiveScene()->tick();
 			}

 			Input::UpdateInput();
            EditorCamera::UpdateCameraTransform();

 			if (!SceneManager::SceneChange() && SceneManager::ActiveScene())
 			{
      		    Renderer::EndBatch();
          		Renderer::Draw();
 			}

            ImGui::Render();
           	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            Renderer::EndDraw();
            glfwPollEvents();
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

    void Engine::LoadRuamConfig()
    {
   		s_config = Serial::DeserializeRuamConfig(SaveSystem::LoadJsonRuamConfig());
    }
}
