#include "Transform.h"

namespace ToolEngine
{
	glm::mat4x4 Transform::getTranslationMatrix()
	{
		glm::mat4x4 matrix =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0 ,1, 0,
			position.x, position.y, position.z, 1
		};
		return matrix;
	}
	glm::mat4x4 Transform::getRotationMatrix()
	{
		Quaternion q = Quaternion::fromEulerRadiansXYZ(rotation);
		float tx = q.x * 2.0f;
		float ty = q.y * 2.0f;
		float tz = q.z * 2.0f;
		float twx = q.w * tx;
		float twy = q.w * ty;
		float twz = q.w * tz;
		float txx = q.x * tx;
		float txy = q.x * ty;
		float txz = q.x * tz;
		float tyy = q.y * ty;
		float tyz = q.y * tz;
		float tzz = q.z * tz;
		glm::mat4x4 matrix =
		{
			1 - (tyy + tzz), txy - twz, txz + twy, 0,
			txy + twz, 1 - (txx + tzz), tyz - twx, 0,
			txz - twy, tyz + twx, 1 - (txx + tyy), 0,
			0, 0, 0, 1
		};
		return matrix;
	}
	glm::mat4x4 Transform::getScaleMatrix()
	{
		glm::mat4x4 matrix =
		{
			scale.x, 0, 0, 0,
			0, scale.y, 0, 0,
			0, 0, scale.z, 0,
			0, 0, 0, 1
		};
		return matrix;
	}
	glm::vec3 Transform::getForward()
	{
		// TODO: forward
		return glm::vec3();
	}
	glm::vec3 Transform::getRight()
	{
		// TODO: right
		return glm::vec3();
	}
	glm::vec3 Transform::getUp()
	{
		glm::vec3 up =
		{
			0, 0, 1
		};
		return up;
	}
	glm::mat4x4 Transform::getModelMatrix()
	{
		glm::mat4x4 matrix = getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();
		return matrix;
	}
}