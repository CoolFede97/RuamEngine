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
    class Engine
    {
    public:
        static void Init();
        static void Start();

        static void CheckIfWantToSaveChanges();

        static void LoadRuamConfig();
        inline static RuamConfig Config() {return s_config;};
        inline static EngineState State() {return s_state;};
    private:
        static bool s_initialized;
        static bool s_started;
        static RuamConfig s_config;
        static EngineState s_state;
    };
}
