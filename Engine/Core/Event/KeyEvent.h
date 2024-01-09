#pragma once

#include "Marco.h"
#include "Event.h"

namespace ToolEngine
{
    class KeyEvent : public Event
    {
    public:
        KeyEvent(int keycode) : m_keycode(keycode) {}
        int getKeyCode() const { return m_keycode; }
        int getCategoryFlags() const override { return EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput; }
    protected:
        int m_keycode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_repeat_count(repeatCount) {}
        int getRepeatCount() const { return m_repeat_count; }
        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_keycode << " (" << m_repeat_count << " repeats)";
            return ss.str();
        }
        static EventType getStaticType() { return EventType::KeyPressed; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "KeyPressed"; }
    private:
        int m_repeat_count;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}
        std::string toString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_keycode;
            return ss.str();
        }
        static EventType getStaticType() { return EventType::KeyReleased; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "KeyReleased"; }
    };
}