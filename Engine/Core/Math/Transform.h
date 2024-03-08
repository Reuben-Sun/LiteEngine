#pragma once

#include "Marco.h"
#include "glm/glm.hpp"
#include "Quaternion.h"

namespace ToolEngine
{
	/// <summary>
	/// Right-handed coordinate system, Z is up
	/// </summary>
	struct Transform
	{
	public:
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		Quaternion rotation = Quaternion::Identity();
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::mat4x4 getTranslationMatrix();
		glm::mat4x4 getRotationMatrix();
		glm::mat4x4 getScaleMatrix();
		glm::vec3 getForward();
		glm::vec3 getRight();
		glm::vec3 getUp();

		glm::mat4x4 getModelMatrix();
	};
}