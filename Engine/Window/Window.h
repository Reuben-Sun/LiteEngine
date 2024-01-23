#pragma once

#include "Marco.h"
#include "Core/Event/Event.h"

namespace ToolEngine
{
    struct WindowProps
    {
        std::string m_title;
        uint32_t m_width;
        uint32_t m_height;

        WindowProps(const std::string& title = "ToolEngine",
            uint32_t width = 1920,
            uint32_t height = 1080)
            : m_title(title), m_width(width), m_height(height)
        {
        }
    };

    class Window
    {
    public:
        Window(const WindowProps& props);
        virtual ~Window();
        std::pair<uint32_t, uint32_t> getExtent() const;

        virtual void tick() = 0;
        using EventCallbackFunc = std::function<void(Event&)>;
        virtual void setEventCallback(const EventCallbackFunc& callback) = 0;
    private:
        uint32_t m_width;
        uint32_t m_height;
    };
}