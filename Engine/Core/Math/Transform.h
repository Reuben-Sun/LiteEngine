#pragma once

#include "Marco.h"
#include "glm/glm.hpp"
#include "Quaternion.h"

namespace ToolEngine
{
	class Transform
	{
	public:


	private:
		glm::vec3 m_position;
		Quaternion m_rotation;
		glm::vec3 m_scale;
	};
}