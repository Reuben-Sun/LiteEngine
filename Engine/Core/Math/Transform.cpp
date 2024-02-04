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
		float tx = rotation.x * 2.0f;
		float ty = rotation.y * 2.0f;
		float tz = rotation.z * 2.0f;
		float twx = rotation.w * tx;
		float twy = rotation.w * ty;
		float twz = rotation.w * tz;
		float txx = rotation.x * tx;
		float txy = rotation.x * ty;
		float txz = rotation.x * tz;
		float tyy = rotation.y * ty;
		float tyz = rotation.y * tz;
		float tzz = rotation.z * tz;
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
		auto v = glm::vec4(0, 0, 1, 0);
		auto m = getModelMatrix();
		return m * v;
	}
	glm::vec3 Transform::getRight()
	{
		auto v = glm::vec4(1, 0, 0, 0);
		auto m = getModelMatrix();
		return m * v;
	}
	glm::vec3 Transform::getUp()
	{
		auto v = glm::vec4(0, 1, 0, 0);
		auto m = getModelMatrix();
		return m * v;
	}
	glm::mat4x4 Transform::getModelMatrix()
	{
		glm::mat4x4 matrix = getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();
		return matrix;
	}
}