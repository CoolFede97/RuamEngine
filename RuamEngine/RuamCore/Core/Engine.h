// #include <iostream>

namespace RuamEngine
{
    class Engine
    {
    public:
        static void Init();
        static void Start();
    private:
        static bool initialized;
        static bool started;
    };
}
