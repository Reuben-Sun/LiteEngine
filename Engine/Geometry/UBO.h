#pragma once

#include "Marco.h"
#include <glm/glm.hpp>
#include "Geometry/Vertex.h"
#include "Geometry/Light.h"

namespace ToolEngine
{
	struct GlobalUBO
	{
		glm::mat4 view_matrix;
		glm::mat4 projection_matrix;
		glm::vec4 camera_position;
		Light directional_light;
	};
}