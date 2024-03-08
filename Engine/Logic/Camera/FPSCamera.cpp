#include "FPSCamera.h"
#include "Core/Time/Time.h"

namespace ToolEngine
{
	FPSCamera::FPSCamera(LogicScene& scene): m_scene(scene)
	{
		CameraComponent camera_component;
		Camera camera;
		camera.transform.position = glm::vec3(3.0f, 3.0f, 3.0f);
		camera.transform.rotation = Quaternion::fromEulerDegreesXYZ(glm::vec3(-63.8f, 0.0f, -133.4f));
		camera.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		camera.fov = glm::radians(45.0f);
		camera.near_plane = 0.1f;
		camera.far_plane = 100.0f;
		camera.view_size = 10;
		camera.camera_speed = 2.0f;
		camera_component.camera = camera;
		auto camera_entity = m_scene.scene_context.create();
		m_scene.scene_context.emplace<CameraComponent>(camera_entity, camera_component);
	}
	FPSCamera::~FPSCamera()
	{
	}
	void FPSCamera::updateMoveState(uint32_t keycode, bool pressed)
	{
		if (keycode == 87)   // 87 is w
		{
			m_forward_state.modify(pressed);
		}
		if (keycode == 83)   // 83 is s
		{
			m_forward_state.modify(!pressed);
		}
		if (keycode == 65)   // 65 is a
		{
			m_right_state.modify(pressed);
		}
		if (keycode == 68)   // 68 is d
		{
			m_right_state.modify(!pressed);
		}
		if (keycode == 32)   // 32 is space
		{
			m_up_state.modify(pressed);
		}
		if (keycode == 340)   // 340 is left shift
		{
			m_up_state.modify(!pressed);
		}
	}
	void FPSCamera::updateRotation(float delta_x, float delta_y)
	{
		auto view = m_scene.scene_context.view<CameraComponent>();
		auto& camera = view.get<CameraComponent>(view.front()).camera;
		auto ui_view = m_scene.scene_context.view<const UIInfoComponent>();
		auto& ui_info = ui_view.get<const UIInfoComponent>(ui_view.front());
		// only mouse on scene image can rotate camera
		if (ui_info.scene_bounding.x < m_current_mouse_x && m_current_mouse_x < ui_info.scene_bounding.z &&
			ui_info.scene_bounding.y < m_current_mouse_y && m_current_mouse_y < ui_info.scene_bounding.w)
		{
			auto euler = camera.transform.rotation.getEulerRandians();
			euler.x += delta_y * 0.001f;
			euler.z += delta_x * 0.001f;
			camera.transform.rotation = Quaternion::fromEulerRadiansXYZ(euler);
		}
	}
	void FPSCamera::updateCameraSpeed(float delta_speed)
	{
		auto view = m_scene.scene_context.view<CameraComponent>();
		auto& camera = view.get<CameraComponent>(view.front()).camera;
		camera.camera_speed = std::clamp(camera.camera_speed + delta_speed, 0.0f, 100.0f);
	}
	void FPSCamera::setCurrentMousePos(uint32_t x, uint32_t y)
	{
		m_current_mouse_x = x;
		m_current_mouse_y = y;
	}
	void FPSCamera::tick()
	{
		OPTICK_EVENT();
		auto view = m_scene.scene_context.view<CameraComponent>();
		auto& camera = view.get<CameraComponent>(view.front()).camera;
			
		if (m_forward_state.value() == 1)
		{
			camera.transform.position -= camera.transform.getForward() * camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		else if (m_forward_state.value() == -1)
		{
			camera.transform.position += camera.transform.getForward() * camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		if (m_right_state.value() == 1)
		{
			camera.transform.position -= camera.transform.getRight() * camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		else if (m_right_state.value() == -1)
		{
			camera.transform.position += camera.transform.getRight() * camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		if (m_up_state.value() == 1)
		{
			camera.transform.position += camera.transform.getUp() * camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		else if (m_up_state.value() == -1)
		{
			camera.transform.position -= camera.transform.getUp() * camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		
	}
}