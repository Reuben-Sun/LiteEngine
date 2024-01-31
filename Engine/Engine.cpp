#include "Engine.h"
#include "Core/Path/Path.h"
#include "Core/Time/Time.h"
#include "Core/Event/EventDispatcher.h"
#include "Geometry/GltfLoader.h"


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
        Transform transform;
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.rotation = Quaternion::Identity();
        transform.scale = glm::vec3(2.0f, 2.0f, 2.0f);
        Material material;
        material.texture_bindings.push_back({ 1, "Calibration.png" });
        scene.mesh_name_list.push_back("Plane");
        scene.mesh_list.push_back(mesh);
        scene.mesh_transform_list.push_back(transform);
        scene.material_list.push_back(material);

        std::string model_path = Path::getInstance().getCurrentPath() + "\\Assets\\cube.gltf";
        std::unique_ptr<GltfLoader> loader = std::make_unique<GltfLoader>(model_path);
        Mesh mesh2;
        mesh2.index_buffer = loader->loaded_index_buffer;
        mesh2.vertex_buffer = loader->loaded_vertex_buffer;
        scene.mesh_name_list.push_back("Cube");
		scene.mesh_list.push_back(mesh2);
        Transform transform2;
        transform2.position = glm::vec3(0.0f, 0.0f, 0.5f);
        transform2.rotation = Quaternion::Identity();
        transform2.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        Material material2;
        material2.texture_bindings.push_back({ 1, "MarblesTiles.jpg" });
        scene.mesh_transform_list.push_back(transform2);
        scene.material_list.push_back(material2);
    }
    void Engine::tick()
    {
        Time::getInstance().tick();
        m_window->tick();
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
        dispatcher.dispatch<KeyPressedEvent>(std::bind(&Engine::onKeyPressed, this, std::placeholders::_1));
        //LOG_INFO("{0}", e.toString());
    }
    bool Engine::onWindowClose(WindowCloseEvent& e)
    {
        m_running = false;
        return true;
    }
    bool Engine::onKeyPressed(KeyPressedEvent& e)
    {
        LOG_INFO("{0}", e.getKeyCode());
        if (e.getKeyCode() == 85)   // 85 is u, TODO: move this to resource
        {
            m_renderer->m_enable_ui = !m_renderer->m_enable_ui;
            m_renderer->resize();
        }
        
        return true;
    }
}