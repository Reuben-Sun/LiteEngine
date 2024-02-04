#include "FPSCamera.h"

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
	void FPSCamera::tick(RenderScene& scene)
	{
		scene.camera = m_camera;
	}
}