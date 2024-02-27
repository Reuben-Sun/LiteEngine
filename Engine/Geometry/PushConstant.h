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

	struct PushConstantWithMaterial
	{
		glm::mat4 model_matrix;
		glm::vec3 base_color;
		float metallic;
		glm::vec3 emission_color;
		float roughness;
	};
}