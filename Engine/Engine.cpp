#include "Engine.h"
#include "Core/Path/Path.h"
#include "Core/Time/Time.h"
#include "Core/Event/EventDispatcher.h"

namespace ToolEngine
{
    Engine::Engine()
    {
    }
    Engine::~Engine()
    {

    }
    void Engine::init(std::string exe_path)
    {
        Path::getInstance().setCurrentPath(exe_path);
        std::string path = Path::getInstance().getCurrentPath();
        LOG_INFO("Workspace path: {0}", path);
        WindowProps props;
        m_window = std::make_unique<GlfwWindow>(props);
        m_window->setEventCallback(std::bind(&Engine::processEvent, this, std::placeholders::_1));
        m_rhi_context = std::make_unique<RHIContext>(*m_window);
        m_renderer = std::make_unique<Renderer>(*m_rhi_context);
    }
    void Engine::tick()
    {
        Time::getInstance().tick();
        m_window->tick();
        m_rhi_context->tick();
        m_renderer->tick();
    }
    void Engine::cleanup()
    {

    }
    void Engine::processEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(std::bind(&Engine::onWindowClose, this, std::placeholders::_1));
        //LOG_INFO("{0}", e.toString());
    }
    bool Engine::onWindowClose(WindowCloseEvent& e)
    {
        m_running = false;
        return true;
    }
}