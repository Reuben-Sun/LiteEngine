#pragma once

#include "Marco.h"
#include "Window.h"
#include <GLFW/glfw3.h>

namespace ToolEngine
{
    class GlfwWindow final : public Window
    {
    public:
        GlfwWindow(const WindowProps& props);
        ~GlfwWindow() override;
        void tick() override;
        void setEventCallback(const EventCallbackFunc& callback) override;

        std::vector<const char*> getRequiredSurfaceExtensions() const;
        GLFWwindow* getHandle() const { return m_window; }
        uint32_t getWidth() const { return m_data.m_width; }
        uint32_t getHeight() const { return m_data.m_height; }
    private:
        GLFWwindow* m_window;

        struct WindowData
        {
            std::string m_title;
            uint32_t m_width;
            uint32_t m_height;
            EventCallbackFunc m_event_callback;
        };

        WindowData m_data;

    };
}