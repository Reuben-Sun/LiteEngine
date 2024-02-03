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

       
        Mesh mesh = Mesh::createPlane();
        Transform transform;
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.scale = glm::vec3(2.0f, 2.0f, 2.0f);
        Material material;
        material.texture_bindings.push_back({ 1, "Calibration.png" });
        scene.mesh_name_list.push_back("Plane");
        scene.mesh_list.push_back(mesh);
        scene.mesh_transform_list.push_back(transform);
        scene.material_list.push_back(material);

        std::string model_path = Path::getInstance().getAssetPath() + "cube.gltf";
        std::unique_ptr<GltfLoader> loader = std::make_unique<GltfLoader>(model_path);
        Mesh mesh2;
        mesh2.index_buffer = loader->loaded_index_buffer;
        mesh2.vertex_buffer = loader->loaded_vertex_buffer;
        scene.mesh_name_list.push_back("Cube");
		scene.mesh_list.push_back(mesh2);
        Transform transform2;
        transform2.position = glm::vec3(0.0f, 0.0f, 0.5f);
        transform2.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        transform2.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        Material material2;
        //material2.texture_bindings.push_back({ 1, "MarblesTiles.jpg" });
        material2.texture_bindings.push_back({ 1, "Cube.png" });
        scene.mesh_transform_list.push_back(transform2);
        scene.material_list.push_back(material2);

        Camera camera;
        camera.transform.position = glm::vec3(5.0f, 0.0f, 1.0f);
        camera.transform.rotation = glm::vec3(-PI/2, 0.0f, -PI/2);
        camera.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
        camera.fov = glm::radians(45.0f); 
        camera.near_plane = 0.1f;
        camera.far_plane = 10.0f;
        camera.view_size = 10;
        scene.camera = camera;
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
        if (e.getKeyCode() == 81)   // 81 is q
        {
            scene.camera.transform.rotation.z -= 0.1f;
        }
        if(e.getKeyCode() == 69)   // 69 is e
		{
            scene.camera.transform.rotation.z += 0.1f;
		}
        if (e.getKeyCode() == 265)   // 265 is up
        {
            scene.camera.transform.rotation.y += 0.1f;
        }
        if (e.getKeyCode() == 264)   // 264 is down
		{
			scene.camera.transform.rotation.y -= 0.1f;
		}
        if (e.getKeyCode() == 263)   // 263 is left
        {
            scene.camera.transform.rotation.x += 0.1f;
        }
        if (e.getKeyCode() == 262)   // 262 is right
		{
            scene.camera.transform.rotation.x -= 0.1f;
		}
        if(e.getKeyCode() == 87)   // 87 is w
		{
            scene.camera.transform.position.x += 1.0f;
		}
        if (e.getKeyCode() == 83)   // 83 is s
        {
            scene.camera.transform.position.x -= 1.0f;
        }
        if (e.getKeyCode() == 65)   // 65 is a
		{
			scene.camera.transform.position.y += 1.0f;
		}
		if (e.getKeyCode() == 68)   // 68 is d
		{
			scene.camera.transform.position.y -= 1.0f;
		}
        return true;
    }
}