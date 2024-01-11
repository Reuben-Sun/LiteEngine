#pragma once
#include "Marco.h"
#include "Core/Event/Event.h"
#include "Core/Event/ApplicationEvent.h"
#include "Window/GlfwWindow.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Render.h"
#include "Geometry/RenderScene.h"

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
        RenderScene scene;
        bool m_running = true;

        bool onWindowClose(WindowCloseEvent& e);
    };
}