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

		glm::vec3 getEulerRandians() const;
		glm::vec3 getEulerDegrees() const;

		static Quaternion Zero();
		static Quaternion Identity();
		// clockwise rotation
		static Quaternion fromRotationX(float angleInRadians);
		static Quaternion fromRotationY(float angleInRadians);
		static Quaternion fromRotationZ(float angleInRadians);
		static Quaternion fromEulerRadiansXYZ(const glm::vec3& eulerRadians);
		static Quaternion fromEulerDegreesXYZ(const glm::vec3& eulerDegrees);

		float x, y, z, w;		
	};
}