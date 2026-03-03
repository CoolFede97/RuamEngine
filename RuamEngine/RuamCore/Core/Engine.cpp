#include "Engine.h"
#include "Renderer.h"
#include "Input.h"
#include "RuamTime.h"
#include "Editor.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "SandboxScene.cpp"
#include "InitialScene.cpp"
#include "EndScene.cpp"
#include "EventManager.h"


namespace RuamEngine
{
    bool Engine::initialized = false;
    bool Engine::started = false;

    void Engine::Init()
    {
        if (initialized)
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

        initialized = true;
    }

    void Engine::Start()
    {
        if (started)
        {
            std::cerr << "Can't call Engine::Init() because the engine is already initialized\n";
            return;
        }
        started = true;
       	unsigned int frameCount = 0;

        SceneManager::AddSceneCreator(2, CreateEndScene);
		SceneManager::AddSceneCreator(1, CreateCFSandboxScene);
		SceneManager::AddSceneCreator(0, CreateInitialScene);

		SceneManager::EnqueueSceneChange(0);


  		while (!Renderer::WindowShouldClose())
  		{

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
        SceneManager::SetActiveScene(nullptr);
    	ImGui_ImplOpenGL3_Shutdown();
    	ImGui_ImplGlfw_Shutdown();
    	ImGui::DestroyContext();
    	Renderer::Shutdown();
    	AudioSystem::shutdown();
    }
}
