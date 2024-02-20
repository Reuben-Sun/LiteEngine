#pragma once

#include "Marco.h"
#include <glm/glm.hpp>

namespace ToolEngine
{
	struct Light 
	{
		glm::vec3 position;
		glm::vec3 color;
		float intensity;
	};

	struct DirectionalLight : public Light
	{
		glm::vec3 direction;
	};

	struct LightUBO
	{
		DirectionalLight directional;
	};
}