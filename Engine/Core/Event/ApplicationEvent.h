#pragma once

#include "Marco.h"
#include "Event.h"

namespace Engine
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}
        unsigned int getWidth() const { return m_width; }
        unsigned int getHeight() const { return m_height; }
        std::string toString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_width << ", " << m_height;
            return ss.str();
        }
        static EventType getStaticType() { return EventType::WindowResize; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "WindowResize"; }
        int getCategoryFlags() const override { return EventCategory::EventCategoryApplication; }
    private:
        unsigned int m_width, m_height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}
        static EventType getStaticType() { return EventType::WindowClose; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "WindowClose"; }
        int getCategoryFlags() const override { return EventCategory::EventCategoryApplication; }
    };

    class AppTickEvent : public Event
    {
    public:
        AppTickEvent() {}
        static EventType getStaticType() { return EventType::AppTick; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "AppTick"; }
        int getCategoryFlags() const override { return EventCategory::EventCategoryApplication; }
    };

    class AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() {}
        static EventType getStaticType() { return EventType::AppUpdate; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "AppUpdate"; }
        int getCategoryFlags() const override { return EventCategory::EventCategoryApplication; }
    };

    class AppRenderEvent : public Event {
    public:
        AppRenderEvent() {}
        static EventType getStaticType() { return EventType::AppRender; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "AppRender"; }
        int getCategoryFlags() const override { return EventCategory::EventCategoryApplication; }
    };
}