#pragma once

#include "Marco.h"
#include "Vertex.h"

namespace ToolEngine
{
	struct Mesh
	{
		std::vector<uint32_t> index_buffer;
		std::vector<Vertex> vertex_buffer;

		static Mesh createPlane();
		static Mesh createLine(float length, glm::vec3 color);
	};
}