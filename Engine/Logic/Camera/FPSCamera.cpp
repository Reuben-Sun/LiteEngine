#include "FPSCamera.h"
#include "Core/Time/Time.h"

namespace ToolEngine
{
	FPSCamera::FPSCamera(RenderScene& scene): m_scene(scene)
	{
		Camera camera;
		camera.transform.position = glm::vec3(3.0f, 3.0f, 3.0f);
		camera.transform.rotation = Quaternion::fromEulerDegreesXYZ(glm::vec3(-63.8f, 0.0f, -133.4f));
		camera.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		camera.fov = glm::radians(45.0f);
		camera.near_plane = 0.1f;
		camera.far_plane = 50.0f;
		camera.view_size = 10;
		camera.camera_speed = 2.0f;

		m_scene.camera = camera;
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
		auto euler = m_scene.camera.transform.rotation.getEulerRandians();
		euler.x += delta_y * 0.001f;
		euler.z += delta_x * 0.001f;
		m_scene.camera.transform.rotation = Quaternion::fromEulerRadiansXYZ(euler);
	}
	void FPSCamera::updateCameraSpeed(float delta_speed)
	{
		m_scene.camera.camera_speed = std::clamp(m_scene.camera.camera_speed + delta_speed, 0.0f, 100.0f);
	}
	void FPSCamera::tick()
	{
		OPTICK_EVENT();
		if (m_forward_state.value() == 1)
		{
			m_scene.camera.transform.position -= m_scene.camera.transform.getForward() * m_scene.camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		else if (m_forward_state.value() == -1)
		{
			m_scene.camera.transform.position += m_scene.camera.transform.getForward() * m_scene.camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		if (m_right_state.value() == 1)
		{
			m_scene.camera.transform.position -= m_scene.camera.transform.getRight() * m_scene.camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		else if (m_right_state.value() == -1)
		{
			m_scene.camera.transform.position += m_scene.camera.transform.getRight() * m_scene.camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		if (m_up_state.value() == 1)
		{
			m_scene.camera.transform.position += m_scene.camera.transform.getUp() * m_scene.camera.camera_speed * Time::getInstance().getDeltaTime();
		}
		else if (m_up_state.value() == -1)
		{
			m_scene.camera.transform.position -= m_scene.camera.transform.getUp() * m_scene.camera.camera_speed * Time::getInstance().getDeltaTime();
		}
	}
}