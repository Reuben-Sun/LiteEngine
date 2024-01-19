#pragma once

#include "Marco.h"
#include "glm/glm.hpp"
#include "Quaternion.h"

namespace ToolEngine
{
	struct Transform
	{
	public:
		glm::vec3 position;
		Quaternion rotation;
		glm::vec3 scale;

		glm::mat4x4 getTranslationMatrix();
		glm::mat4x4 getRotationMatrix();
		glm::mat4x4 getScaleMatrix();
		glm::vec3 getForward();
		glm::vec3 getRight();
		glm::vec3 getUp();

		glm::mat4x4 getModelMatrix();
	};
}