#pragma once

#include "Marco.h"
#include "Event.h"

namespace Engine
{
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y) : m_mouse_x(x), m_mouse_y(y) {}
        float getX() const { return m_mouse_x; }
        float getY() const { return m_mouse_y; }
        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_mouse_x << ", " << m_mouse_y;
            return ss.str();
        }
        static EventType getStaticType() { return EventType::MouseMoved; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "MouseMoved"; }
        int getCategoryFlags() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }

    private:
        float m_mouse_x, m_mouse_y;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float x_offset, float y_offset) : m_x_offset(x_offset), m_y_offset(y_offset) {}
        float getXOffset() const { return m_x_offset; }
        float getYOffset() const { return m_y_offset; }
        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
            return ss.str();
        }
        static EventType getStaticType() { return EventType::MouseScrolled; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "MouseScrolled"; }
        int getCategoryFlags() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }

    private:
        float m_x_offset, m_y_offset;
    };

    class MouseButtonEvent : public Event
    {
    public:
        MouseButtonEvent(int button) : m_button(button) {}
        int getMouseButton() const { return m_button; }
        int getCategoryFlags() const override { return EventCategory::EventCategoryMouseButton | EventCategory::EventCategoryInput; }
    protected:
        int m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}
        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_button;
            return ss.str();
        }
        static EventType getStaticType() { return EventType::MouseButtonPressed; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "MouseButtonPressed"; }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}
        std::string toString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_button;
            return ss.str();
        }
        static EventType getStaticType() { return EventType::MouseButtonReleased; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "MouseButtonReleased"; }
    };
}