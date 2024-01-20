#include "Engine.h"
#include "Core/Path/Path.h"
#include "Core/Time/Time.h"
#include "Core/Event/EventDispatcher.h"


namespace ToolEngine
{
    Engine::Engine()
    {
    }
    Engine::~Engine()
    {

    }
    void Engine::init(std::string exe_path)
    {
        Path::getInstance().setCurrentPath(exe_path);
        std::string path = Path::getInstance().getCurrentPath();
        LOG_INFO("Workspace path: {0}", path);
        WindowProps props;
        m_window = std::make_unique<GlfwWindow>(props);
        m_window->setEventCallback(std::bind(&Engine::processEvent, this, std::placeholders::_1));
        m_rhi_context = std::make_unique<RHIContext>(*m_window);
        m_renderer = std::make_unique<Renderer>(*m_rhi_context);

        std::vector<uint32_t> plane_index_buffer = { 0, 1, 2, 2, 3, 0 };
        const std::vector<Vertex> plane_vertex_buffer =
        {
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
            {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
        };
        Mesh mesh;
        mesh.index_buffer = plane_index_buffer;
        mesh.vertex_buffer = plane_vertex_buffer;
        scene.mesh_list.push_back(mesh);
        Transform transform;
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.rotation = Quaternion::Identity();
        transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        scene.mesh_transform_list.push_back(transform);

        Transform transform2;
        transform2.position = glm::vec3(0.0f, 0.0f, 1.0f);
        transform2.rotation = Quaternion::Identity();
        transform2.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        scene.mesh_list.push_back(mesh);
        scene.mesh_transform_list.push_back(transform2);
    }
    void Engine::tick()
    {
        Time::getInstance().tick();
        m_window->tick();
        for (Layer* layer : m_layer_stack)
        {
            layer->onUpdate();
        }
        m_rhi_context->tick();
        m_renderer->tick(scene);
    }
    void Engine::cleanup()
    {

    }
    void Engine::processEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(std::bind(&Engine::onWindowClose, this, std::placeholders::_1));
        //LOG_INFO("{0}", e.toString());
        for (auto it = m_layer_stack.end(); it != m_layer_stack.begin();)
        {
            it--;
            (*it)->onEvent(e);
            if (e.had_handled)
            {
                break;
            }
        }
    }
    bool Engine::onWindowClose(WindowCloseEvent& e)
    {
        m_running = false;
        return true;
    }
}