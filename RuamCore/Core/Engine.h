// #include <iostream>
#include "Serial.h"
#include "nlohmann/json.hpp"
#include <vector>
#include "RuamConfig.h"

namespace RuamEngine
{
    class Engine
    {
    public:
        static void Init();
        static void Start();

        static void CheckIfWantToSaveChanges();

        static void LoadRuamConfig();
        static RuamConfig Config();
    private:
        static bool s_initialized;
        static bool s_started;
        static RuamConfig s_config;
    };
}
