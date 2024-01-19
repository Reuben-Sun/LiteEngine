#pragma once

#include "Marco.h"
#include "glm/glm.hpp"

namespace ToolEngine
{
	class Quaternion
	{
	public:
		Quaternion() = default;
		Quaternion(const Quaternion& other);
		explicit Quaternion(float x, float y, float z, float w);
		explicit Quaternion(glm::vec3& v, float w);

		static Quaternion Zero();
		static Quaternion Identity();

		static Quaternion fromRotationX(float angleInRadians);
		static Quaternion fromRotationY(float angleInRadians);
		static Quaternion fromRotationZ(float angleInRadians);
		static Quaternion fromEulerRadiansXYZ(const glm::vec3& eulerRadians);

		float X() const { return m_x; }
		float Y() const { return m_y; }
		float Z() const { return m_z; }
		float W() const { return m_w; }
	private:
		float m_x, m_y, m_z, m_w;
	};
}