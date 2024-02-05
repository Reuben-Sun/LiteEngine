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

		virtual float getVolume() = 0;
	};

	struct BoundingBox : public Bounding
	{
		glm::vec3 half_length;

		virtual float getVolume() override
		{
			return half_length.x * half_length.y * half_length.z * 8;
		}
	};

	struct BoundingSphere : public Bounding
	{
		float radius;

		virtual float getVolume() override
		{
			return 4.0f / 3.0f * PI * radius * radius * radius;
		}
	};
}