#include "GPContext.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
    GPContext::GPContext()
    {
        m_scene_manager = std::make_unique<SceneManager>(m_scene);
        m_fps_camera = std::make_unique<FPSCamera>(m_scene);
        //m_physics_manager = std::make_unique<PhysicsManager>(m_scene);
    }
    GPContext::~GPContext()
    {
    }
    void GPContext::tick()
    {
        m_scene_manager->tick();
        m_fps_camera->tick();
        //m_physics_manager->tick();
    }
}