#include "Engine.h"
#include "Core/Path/Path.h"
#include "Core/Time/Time.h"
#include "Core/Event/EventDispatcher.h"
#include "Geometry/GltfLoader.h"


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
        m_gp_context = std::make_unique<GPContext>();
        m_rhi_context = std::make_unique<RHIContext>(*m_window);
        m_render_context = std::make_unique<RenderContext>(*m_rhi_context);
        m_render_context->setUICallbackFunc(std::bind(&Engine::processUIEvent, this, std::placeholders::_1));
    }
    void Engine::tick()
    {
        OPTICK_FRAME("MainThread");
        Time::getInstance().tick();
        m_window->tick();
        m_rhi_context->tick();
        LogicScene& scene = m_gp_context->getLogicScene();
        m_render_context->tick(scene);
        m_gp_context->tick();
    }
    void Engine::cleanup()
    {

    }
    void Engine::processEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(std::bind(&Engine::onWindowClose, this, std::placeholders::_1));
        dispatcher.dispatch<KeyPressedEvent>(std::bind(&Engine::onKeyPressed, this, std::placeholders::_1));
        dispatcher.dispatch<KeyReleasedEvent>(std::bind(&Engine::onKeyReleased, this, std::placeholders::_1));
        dispatcher.dispatch<MouseMovedEvent>(std::bind(&Engine::onMouseMoved, this, std::placeholders::_1));
        dispatcher.dispatch<MouseButtonPressedEvent>(std::bind(&Engine::onMouseButtonPressed, this, std::placeholders::_1));
        dispatcher.dispatch<MouseButtonReleasedEvent>(std::bind(&Engine::onMouseButtonReleased, this, std::placeholders::_1));
        dispatcher.dispatch<MouseScrolledEvent>(std::bind(&Engine::onMouseScrolled, this, std::placeholders::_1));
    }
    void Engine::processUIEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
		dispatcher.dispatch<UIReloadSceneEvent>(std::bind(&Engine::onUIReloadScene, this, std::placeholders::_1));
    }
    bool Engine::onWindowClose(WindowCloseEvent& e)
    {
        m_running = false;
        return true;
    }
    bool Engine::onKeyPressed(KeyPressedEvent& e)
    {
        //LOG_INFO("{0}", e.getKeyCode());

        if (e.getKeyCode() == 85)   // 85 is u, TODO: move this to resource
        {
            m_render_context->setFullScreen();
        }
        
        m_gp_context->m_fps_camera->updateMoveState(e.getKeyCode(), true);
        return true;
    }
    bool Engine::onKeyReleased(KeyReleasedEvent& e)
    {
        m_gp_context->m_fps_camera->updateMoveState(e.getKeyCode(), false);
        return true;
    }
    bool Engine::onMouseMoved(MouseMovedEvent& e)
    {
        m_gp_context->setCurrentMousePosition(e.getX(), e.getY());
        if(m_mouse_button_state == 1)
		{
            m_mouse_button_state = 2;
            m_last_mouse_x = e.getX();
            m_last_mouse_y = e.getY();
            return true;
		}
        if(m_mouse_button_state == 2)
		{
			float delta_x = e.getX() - m_last_mouse_x;
            float delta_y = e.getY() - m_last_mouse_y;
            m_last_mouse_x = e.getX();
			m_last_mouse_y = e.getY();
            m_gp_context->m_fps_camera->updateRotation(delta_x, delta_y);
		}
        return true;
    }
    bool Engine::onMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.getMouseButton() == 1)    // 1 is right mouse button
        {
            m_mouse_button_state = 1;
        }
        return true;
    }
    bool Engine::onMouseButtonReleased(MouseButtonReleasedEvent& e)
    {
        if (e.getMouseButton() == 1)    // 1 is right mouse button
        {
            m_mouse_button_state = 0;
        }
        return true;
    }
    bool Engine::onMouseScrolled(MouseScrolledEvent& e)
    {
        if (m_mouse_button_state == 2)  // scroll when press right mouse button can change camera speed
        {
            m_gp_context->m_fps_camera->updateCameraSpeed(e.getYOffset());
        }
        
        return true;
    }
    bool Engine::onUIReloadScene(UIReloadSceneEvent& e)
    {
        LOG_INFO("Reload Scene");
        m_gp_context->reloadScene(e.getSceneName());
        return false;
    }
}