#include "FPSCamera.h"
#include "Core/Time/Time.h"

namespace ToolEngine
{
	FPSCamera::FPSCamera()
	{
		m_camera.transform.position = glm::vec3(3.0f, 3.0f, 3.0f);
		m_camera.transform.rotation = Quaternion::fromEulerDegreesXYZ(glm::vec3(-63.8f, 0.0f, -133.4f));
		m_camera.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		m_camera.fov = glm::radians(45.0f);
		m_camera.near_plane = 0.1f;
		m_camera.far_plane = 10.0f;
		m_camera.view_size = 10;
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
		auto euler = m_camera.transform.rotation.getEulerRandians();
		euler.x += delta_y * 0.001f;
		euler.z += delta_x * 0.001f;
		m_camera.transform.rotation = Quaternion::fromEulerRadiansXYZ(euler);
	}
	void FPSCamera::tick(RenderScene& scene)
	{
		if (m_forward_state.value() == 1)
		{
			m_camera.transform.position -= m_camera.transform.getForward() * m_camera_speed * Time::getInstance().getDeltaTime();
		}
		else if (m_forward_state.value() == -1)
		{
			m_camera.transform.position += m_camera.transform.getForward() * m_camera_speed * Time::getInstance().getDeltaTime();
		}
		if (m_right_state.value() == 1)
		{
			m_camera.transform.position -= m_camera.transform.getRight() * m_camera_speed * Time::getInstance().getDeltaTime();
		}
		else if (m_right_state.value() == -1)
		{
			m_camera.transform.position += m_camera.transform.getRight() * m_camera_speed * Time::getInstance().getDeltaTime();
		}
		if (m_up_state.value() == 1)
		{
			m_camera.transform.position += m_camera.transform.getUp() * m_camera_speed * Time::getInstance().getDeltaTime();
		}
		else if (m_up_state.value() == -1)
		{
			m_camera.transform.position -= m_camera.transform.getUp() * m_camera_speed * Time::getInstance().getDeltaTime();
		}
		scene.camera = m_camera;
	}
}