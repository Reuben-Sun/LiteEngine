#pragma once

#include "Marco.h"
#include <glm/glm.hpp>
#include "Geometry/Vertex.h"
#include "Geometry/Light.h"

namespace ToolEngine
{
	struct PushConstant
	{
		glm::mat4 model_matrix;
	};
}