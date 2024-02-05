#pragma once

#include "Marco.h"
#include <glm/glm.hpp>
#include "Core/Math/Transform.h"

namespace ToolEngine
{
	enum class BoundingType
	{
		Box,
		Sphere
	};

	struct Bounding
	{
		glm::vec3 position;
		glm::vec3 data;
		BoundingType type;
		float getVolume()
		{
			if (type == BoundingType::Box)
			{
				return data.x * data.y * data.z * 8;
			}
			else if(type == BoundingType::Sphere)
			{
				return 4.0f / 3.0f * PI * data.x * data.y * data.z;
			}
			return 0;
		}
	};
}