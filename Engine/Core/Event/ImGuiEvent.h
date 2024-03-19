#pragma once

#include "Marco.h"
#include "Event.h"

namespace ToolEngine
{
	class UIReloadSceneEvent : public Event
	{
    public:
        UIReloadSceneEvent(std::string scene_name) : m_scene_name(scene_name) {}
        std::string getSceneName() const { return m_scene_name; }
        std::string toString() const override
        {
            std::stringstream ss;
            ss << "UIReloadSceneEvent: " << m_scene_name;
            return ss.str();
        }
        static EventType getStaticType() { return EventType::ImGuiEvent; }
        EventType getEventType() const override { return getStaticType(); }
        std::string getName() const override { return "UIReloadScene"; }
        int getCategoryFlags() const override { return EventCategory::EventCategoryUI; }

    private:
        std::string m_scene_name;
	};
}