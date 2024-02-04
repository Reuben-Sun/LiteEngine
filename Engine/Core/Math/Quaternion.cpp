#include "Quaternion.h"

namespace ToolEngine
{
	Quaternion::Quaternion(const Quaternion& other): x(other.x), y(other.y), z(other.z), w(other.w)
	{
	}
	Quaternion::Quaternion(float x, float y, float z, float w): x(x), y(y), z(z), w(w)
	{
	}
	Quaternion::Quaternion(glm::vec3& v, float w): x(v.x), y(v.y), z(v.z), w(w)
	{
	}

	Quaternion Quaternion::operator*(const Quaternion& other) const
	{
		return Quaternion(
			(y * other.z) - (z * other.y) + (w * other.x) + (x * other.w),
			(z * other.x) - (x * other.z) + (w * other.y) + (y * other.w),
			(x * other.y) - (y * other.x) + (w * other.z) + (z * other.w),
			(w * other.w) - (x * other.x) - (y * other.y) - (z * other.z)
		);
	}

	Quaternion& Quaternion::operator*=(const Quaternion& other)
	{
		*this = *this * other;
		return *this;
	}

	glm::vec3 Quaternion::getEulerRandians() const
	{
		const float sinp = 2.0f * (w * y + z * x);

		if (sinp* sinp < 0.5f)
		{
			// roll (x-axis rotation)
			const float roll = glm::atan(2.0f * (w * x - z * y), 1.0f - 2.0f * (x * x + y * y));

			// pitch (y-axis rotation)
			const float pitch = asinf(sinp);

			// yaw (z-axis rotation)
			const float yaw = glm::atan(2.0f * (w * z - x * y), 1.0f - 2.0f * (y * y + z * z));

			return glm::vec3(roll, pitch, yaw);
		}

		// find the pitch from its cosine instead, to avoid issues with sensitivity of asin when the sine value is close to 1
		else
		{
			const float sign = sinp > 0.0f ? 1.0f : -1.0f;
			const float m12 = 2.0f * (z * y - w * x);
			const float m22 = 1.0f - 2.0f * (x * x + y * y);
			const float cospSq = m12 * m12 + m22 * m22;
			const float cosp = glm::sqrt(cospSq);
			const float pitch = sign * acosf(cosp);
			if (cospSq > 0.0001f)
			{
				const float roll = glm::atan(-m12, m22);
				const float yaw = glm::atan(2.0f * (w * z - x * y), 1.0f - 2.0f * (y * y + z * z));
				return glm::vec3(roll, pitch, yaw);
			}
			// if the pitch is close enough to +-pi/2, use a different approach because the terms used above lose roll and yaw information
			else
			{
				const float m21 = 2.0f * (y * z + x * w);
				const float m11 = 1.0f - 2.0f * (x * x + z * z);
				const float roll = glm::atan(m21, m11);
				return glm::vec3(roll, pitch, 0.0f);
			}
		}
	}

	glm::vec3 Quaternion::getEulerDegrees() const
	{
		auto euler = getEulerRandians();
		return glm::vec3(glm::degrees(euler[0]), glm::degrees(euler[1]), glm::degrees(euler[2]));
	}

	Quaternion Quaternion::Zero()
	{
		return Quaternion(0, 0, 0, 0);
	}
	Quaternion Quaternion::Identity()
	{
		return Quaternion(0, 0, 0, 1);
	}
	Quaternion Quaternion::fromRotationX(float angleInRadians)
	{
		const float halfAngle = 0.5f * angleInRadians;
		return Quaternion(sin(halfAngle), 0, 0, cos(halfAngle));
	}
	Quaternion Quaternion::fromRotationY(float angleInRadians)
	{
		const float halfAngle = 0.5f * angleInRadians;
		return Quaternion(0, sin(halfAngle), 0, cos(halfAngle));
	}
	Quaternion Quaternion::fromRotationZ(float angleInRadians)
	{
		const float halfAngle = 0.5f * angleInRadians;
		return Quaternion(0, 0, sin(halfAngle), cos(halfAngle));
	}
	Quaternion Quaternion::fromEulerRadiansXYZ(const glm::vec3& eulerRadians)
	{
		const float halfX = 0.5f * eulerRadians.x;
		const float halfY = 0.5f * eulerRadians.y;
		const float halfZ = 0.5f * eulerRadians.z;
		const float sx = sin(halfX);
		const float cx = cos(halfX);
		const float sy = sin(halfY);
		const float cy = cos(halfY);
		const float sz = sin(halfZ);
		const float cz = cos(halfZ);
		return Quaternion(
			cx * sy * sz + sx * cy * cz,
			cx * sy * cz - sx * cy * sz,
			cx * cy * sz + sx * sy * cz,
			cx * cy * cz - sx * sy * sz
		);
	}
}