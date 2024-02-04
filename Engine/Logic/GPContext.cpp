#include "GPContext.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
    GPContext::GPContext()
    {
        m_scene_manager = std::make_unique<SceneManager>();
        Camera camera;
        camera.transform.position = glm::vec3(3.0f, 3.0f, 3.0f);
        camera.transform.rotation = Quaternion::fromEulerDegreesXYZ(glm::vec3(-63.8f, 0.0f, -133.4f));
        camera.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        camera.fov = glm::radians(45.0f);
        camera.near_plane = 0.1f;
        camera.far_plane = 10.0f;
        camera.view_size = 10;
        m_scene.camera = camera; 
    }
    GPContext::~GPContext()
    {
    }
    void GPContext::tick()
    {
        m_scene_manager->tick(m_scene);
    }
}