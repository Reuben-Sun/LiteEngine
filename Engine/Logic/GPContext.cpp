#include "GPContext.h"
#include "Core/Path/Path.h"
#include "Core/Time/Time.h"

namespace ToolEngine
{
    GPContext::GPContext()
    {
        // scene manager need first init
        m_scene_manager = std::make_unique<SceneManager>(m_logic_scene);
        m_fps_camera = std::make_unique<FPSCamera>(m_logic_scene);
        m_physics_manager = std::make_unique<PhysicsManager>(m_logic_scene);
    }
    GPContext::~GPContext()
    {
    }
    void GPContext::tick()
    {
        OPTICK_EVENT();
        float fixed_delta_time = getFixedDeltaTime();
        m_accumulator += Time::getInstance().getDeltaTime();
        if (m_accumulator > fixed_delta_time)
        {
            while (m_accumulator > fixed_delta_time)
            {
                m_accumulator -= fixed_delta_time;
            }
            m_physics_manager->tick(fixed_delta_time);
		}
        m_fps_camera->tick();
        
        // scene manager need last tick
        m_scene_manager->tick(m_scene);
    }
}