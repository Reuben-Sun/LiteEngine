#pragma once
#include "Marco.h"
#include "Core/Event/Event.h"
#include "Core/Event/ApplicationEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Window/GlfwWindow.h"
#include "RHI/Public/RHIContext.h"
#include "Render.h"
#include "Geometry/RenderScene.h"
#include "Logic/GPContext.h"


namespace ToolEngine
{
    class Engine final
    {
    public:
        Engine();
        ~Engine();

        void init(std::string exe_path);
        void tick();
        void cleanup();
        void processEvent(Event& e);

        bool isRunning() const { return m_running; }

    private:
        std::unique_ptr<GlfwWindow> m_window;
        std::unique_ptr<RHIContext> m_rhi_context;
        std::unique_ptr<Renderer> m_renderer;
        std::unique_ptr<GPContext> m_gp_context;
        bool m_running = true;

        uint32_t m_mouse_button_state = 0;  // 0 is disable, 1 is start, 2 is running
        float m_last_mouse_x = 0.0f;
        float m_last_mouse_y = 0.0f;

        bool onWindowClose(WindowCloseEvent& e);
        bool onKeyPressed(KeyPressedEvent& e);
        bool onKeyReleased(KeyReleasedEvent& e);
        bool OnMouseMoved(MouseMovedEvent& e);
        bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
        bool OnMouseScrolled(MouseScrolledEvent& e);
    };
}