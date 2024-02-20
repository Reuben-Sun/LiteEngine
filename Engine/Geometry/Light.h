#pragma once

#include "Marco.h"
#include <glm/glm.hpp>

namespace ToolEngine
{
	struct Light
	{
		glm::vec3 position;
		float intensity;
		glm::vec4 color;
		glm::vec4 direction;
	};
}