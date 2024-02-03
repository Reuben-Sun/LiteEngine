#pragma once

#include "Marco.h"
#include "glm/glm.hpp"

namespace ToolEngine
{
	struct Quaternion
	{
	public:
		Quaternion() = default;
		Quaternion(const Quaternion& other);
		explicit Quaternion(float x, float y, float z, float w);
		explicit Quaternion(glm::vec3& v, float w);
		// Quaternion * mean blend rotation
		Quaternion operator*(const Quaternion& other) const;
		Quaternion& operator*=(const Quaternion& other);

		static Quaternion Zero();
		static Quaternion Identity();

		static Quaternion fromRotationX(float angleInRadians);
		static Quaternion fromRotationY(float angleInRadians);
		static Quaternion fromRotationZ(float angleInRadians);
		static Quaternion fromEulerRadiansXYZ(const glm::vec3& eulerRadians);

		float x, y, z, w;		
	};
}