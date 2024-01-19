#include "Quaternion.h"

namespace ToolEngine
{
	Quaternion::Quaternion(const Quaternion& other): m_x(other.m_x), m_y(other.m_y), m_z(other.m_z), m_w(other.m_w)
	{
	}
	Quaternion::Quaternion(float x, float y, float z, float w): m_x(x), m_y(y), m_z(z), m_w(w)
	{
	}
	Quaternion::Quaternion(glm::vec3& v, float w): m_x(v.x), m_y(v.y), m_z(v.z), m_w(w)
	{
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