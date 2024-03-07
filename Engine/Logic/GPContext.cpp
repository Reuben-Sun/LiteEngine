#include "GPContext.h"
#include "Core/Path/Path.h"
#include "Core/Time/Time.h"

namespace ToolEngine
{
    GPContext::GPContext()
    {
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
        m_physics_manager->tick();
        m_fps_camera->tick();
        m_scene_manager->tick();
    }
}