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
        transform.rotation = Quaternion::Identity();
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
        transform2.rotation = Quaternion::Identity();
        transform2.scale = glm::vec3(0.5f, 0.5f, 0.5f);
        Material material2;
        material2.texture_bindings.push_back({ 1, "MarblesTiles.jpg" });
        //material2.texture_bindings.push_back({ 1, "Cube.png" });
        scene.mesh_transform_list.push_back(transform2);
        scene.material_list.push_back(material2);

        Camera camera;
        camera.transform.position = glm::vec3(3.0f, 3.0f, 3.0f);
        camera.transform.rotation = Quaternion::fromEulerDegreesXYZ(glm::vec3(-63.8f, 0.0f, -133.4f));
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
        if (m_forward_state.value() == 1)
        {
            scene.camera.transform.position -= scene.camera.transform.getForward() * m_camera_speed * Time::getInstance().getDeltaTime();
        }
        else if (m_forward_state.value() == -1)
        {
            scene.camera.transform.position += scene.camera.transform.getForward() * m_camera_speed * Time::getInstance().getDeltaTime();
        }
        if (m_right_state.value() == 1)
		{
			scene.camera.transform.position -= scene.camera.transform.getRight() * m_camera_speed * Time::getInstance().getDeltaTime();
		}
		else if (m_right_state.value() == -1)
		{
			scene.camera.transform.position += scene.camera.transform.getRight() * m_camera_speed * Time::getInstance().getDeltaTime();
		}
        if (m_up_state.value() == 1)
        {
			scene.camera.transform.position += scene.camera.transform.getUp() * m_camera_speed * Time::getInstance().getDeltaTime();
		}
        else if (m_up_state.value() == -1)
        {
			scene.camera.transform.position -= scene.camera.transform.getUp() * m_camera_speed * Time::getInstance().getDeltaTime();
		}
    }
    void Engine::cleanup()
    {

    }
    void Engine::processEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(std::bind(&Engine::onWindowClose, this, std::placeholders::_1));
        dispatcher.dispatch<KeyPressedEvent>(std::bind(&Engine::onKeyPressed, this, std::placeholders::_1));
        dispatcher.dispatch<KeyReleasedEvent>(std::bind(&Engine::onKeyReleased, this, std::placeholders::_1));
        dispatcher.dispatch<MouseMovedEvent>(std::bind(&Engine::OnMouseMoved, this, std::placeholders::_1));
        dispatcher.dispatch<MouseButtonPressedEvent>(std::bind(&Engine::OnMouseButtonPressed, this, std::placeholders::_1));
        dispatcher.dispatch<MouseButtonReleasedEvent>(std::bind(&Engine::OnMouseButtonReleased, this, std::placeholders::_1));
        
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
        
        if(e.getKeyCode() == 87)   // 87 is w
		{
            m_forward_state.add();
		}
        if (e.getKeyCode() == 83)   // 83 is s
        {
            m_forward_state.del();
        }
        if (e.getKeyCode() == 65)   // 65 is a
		{
            m_right_state.add();
		}
		if (e.getKeyCode() == 68)   // 68 is d
		{
            m_right_state.del();
		}
        if (e.getKeyCode() == 32)   // 32 is space
        {
            m_up_state.add();
        }
        if (e.getKeyCode() == 340)   // 340 is left shift
        {
            m_up_state.del();
        }
        return true;
    }
    bool Engine::onKeyReleased(KeyReleasedEvent& e)
    {
        

        if (e.getKeyCode() == 87)   // 87 is w
        {
            m_forward_state.del();
            
        }
        if (e.getKeyCode() == 83)   // 83 is s
        {
            m_forward_state.add();
        }

        if (e.getKeyCode() == 65)   // 65 is a
		{
            m_right_state.del();
		}

		if (e.getKeyCode() == 68)   // 68 is d
        {
            m_right_state.add();
        }
        if (e.getKeyCode() == 32)   // 32 is space
        {
            m_up_state.del();
		}
        if (e.getKeyCode() == 340)   // 340 is left shift
        {
            m_up_state.add();
        }
        return true;
    }
    bool Engine::OnMouseMoved(MouseMovedEvent& e)
    {
        if(m_mouse_button_state == 1)
		{
            m_mouse_button_state = 2;
            m_last_mouse_x = e.getX();
            m_last_mouse_y = e.getY();
            return true;
		}
        if(m_mouse_button_state == 2)
		{
			float delta_x = e.getX() - m_last_mouse_x;
            float delta_y = e.getY() - m_last_mouse_y;
            m_last_mouse_x = e.getX();
			m_last_mouse_y = e.getY();
			
            auto euler = scene.camera.transform.rotation.getEulerRandians();
            euler.x += delta_y * 0.001f;
            euler.z += delta_x * 0.001f;
            scene.camera.transform.rotation = Quaternion::fromEulerRadiansXYZ(euler);
		}
        return true;
    }
    bool Engine::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if (e.getMouseButton() == 1)    // 1 is right mouse button
        {
            m_mouse_button_state = 1;
        }
        return true;
    }
    bool Engine::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
    {
        if (e.getMouseButton() == 1)    // 1 is right mouse button
        {
            m_mouse_button_state = 0;
        }
        return true;
    }
}