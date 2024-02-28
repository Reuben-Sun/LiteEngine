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
		glm::vec3 base_color;
		float metallic;
		glm::vec3 emission_color;
		float roughness;
		uint32_t texture_enable;
		uint32_t debug_mode;
		glm::vec2 padding;
	};
}