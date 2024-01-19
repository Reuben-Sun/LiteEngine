#include "Transform.h"

namespace ToolEngine
{
	glm::mat4x4 Transform::getTranslationMatrix()
	{
		return glm::mat4x4();
	}
	glm::mat4x4 Transform::getRotationMatrix()
	{
		return glm::mat4x4();
	}
	glm::mat4x4 Transform::getScaleMatrix()
	{
		return glm::mat4x4();
	}
	glm::vec3 Transform::getForward()
	{
		return glm::vec3();
	}
	glm::vec3 Transform::getRight()
	{
		return glm::vec3();
	}
	glm::vec3 Transform::getUp()
	{
		return glm::vec3();
	}
	glm::mat4x4 Transform::getModelMatrix()
	{
		return glm::mat4x4();
	}
}