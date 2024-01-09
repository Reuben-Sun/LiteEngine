#include "Engine.h"

int main(int argc, char* argv[])
{
    ToolEngine::Engine engine;
    engine.init(argv[0]);
    while (engine.isRunning())
    {
        engine.tick();
    }
    engine.cleanup();
    return 0;
}