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
	std::vector<float> Quaternion::toVector() const
	{
		return std::vector<float>{ x, y, z, w };
	}
}