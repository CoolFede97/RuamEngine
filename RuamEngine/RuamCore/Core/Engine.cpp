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

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "SandboxScene.cpp"
#include "InitialScene.cpp"
#include "EndScene.cpp"


namespace RuamEngine
{
    bool Engine::s_initialized = false;
    bool Engine::s_started = false;
    RuamConfig Engine::s_config;

    void Engine::Init()
    {
        if (s_initialized)
        {
            std::cerr << "Can't call Engine::SetUp() because the engine is already set up\n";
            return;
        }
        Renderer::Init();
    	AudioSystem::init();

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

   		LoadRuamConfig();
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
		SceneManager::EnqueueSceneChange(SceneManager::scenes()[0]);


  		while (!Renderer::WindowShouldClose())
  		{
    		CheckIfWantToSaveChanges();
 			SceneManager::ApplyPendingSceneChange();
  		    // std::cout << "Frame count: " << frameCount++ << "\n";

 			// ImGUI
 			ImGui_ImplOpenGL3_NewFrame();
 			ImGui_ImplGlfw_NewFrame();
 			ImGui::NewFrame();

    		Editor::UpdateHierarchy();
            Editor::UpdateInspector();
 			// Time
 			Time::Update();

 			Renderer::ClearScreen();
 			Renderer::BeginBatch();

 			EventManager::HandleEvents();

            // std::cout << "Frame count: " << frameCount++ << "\n";
 			if (SceneManager::ActiveScene() != nullptr)
 			{
				SceneManager::ActiveScene()->tick();
 			}

 			Input::UpdateInput();

 			if (!SceneManager::SceneChange())
 			{
      		    Renderer::EndBatch();
          		Renderer::Draw();
 			}
 			else std::cout << "No se llamo a draw porque se acaba de cambiar de escena\n";

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
    	AudioSystem::shutdown();
    }

    void Engine::CheckIfWantToSaveChanges()
    {
    	if (Input::GetKey(KeyCode::LeftControl_Key) && Input::GetKeyDown(KeyCode::S_Key))
     	{
      		SaveSystem::SaveRuamConfig();

			SaveSystem::SaveJsonScene(Serial::Serialize(SceneManager::ActiveScene()));
			std::cout << "Changes saved successfully!\n";
      	}
    }

    void Engine::LoadRuamConfig()
    {
   		s_config = Serial::DeserializeRuamConfig(SaveSystem::LoadJsonRuamConfig());
    }

    RuamConfig Engine::Config()
    {
    	return s_config;
    }
}
