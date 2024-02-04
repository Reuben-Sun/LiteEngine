#pragma once

#include "Marco.h"
#include "Core/Math/Transform.h"

namespace ToolEngine
{
	struct Camera
	{
		float aspect;
		float view_size;
		float near_plane;
		float far_plane;
		float fov;
		Transform transform;

		float camera_speed;

		glm::mat4 getProjectionMatrix();
		glm::mat4x4 getViewMatrix();
	};
}