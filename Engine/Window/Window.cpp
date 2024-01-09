#include "Window.h"

namespace ToolEngine
{
    Window::Window(const WindowProps& props)
    {
        m_width = props.m_width;
        m_height = props.m_height;
    }
    Window::~Window()
    {
    }
    std::pair<uint32_t, uint32_t> Window::getExtent() const
    {
        return std::make_pair(m_width, m_height);
    }
}