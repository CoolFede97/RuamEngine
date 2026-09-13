// #include <iostream>
#include "nlohmann/json.hpp"
#include "RuamConfig.h"

namespace RuamEngine
{
    enum EngineState
    {
        GameMode,
        EditorMode
    };

    class Scene;

    class Engine
    {
    public:
        static void Init();
        static void Start();

        static void UpdateEngineState(Scene* scene);

        static bool CheckIfInitiable();

        static void CheckIfWantToSaveChanges();
        static void ImGuiStartNewFrame();

        static void LoadRuamConfig();
        inline static RuamConfig Config() {return s_config;}
        inline static EngineState State() {return s_state;}
        inline static bool ShuttingDown() { return s_shuttingDown; }
    private:
        static void ImGuiShutdown();

        static bool s_initialized; // set up
        static bool s_started; // running
        static bool s_shuttingDown;
        static RuamConfig s_config;
        static EngineState s_state;
    };
}
