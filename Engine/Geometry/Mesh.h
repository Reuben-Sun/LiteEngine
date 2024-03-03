#pragma once

#include "Marco.h"
#include "Vertex.h"

namespace ToolEngine
{
	struct SubMesh
	{
		std::vector<uint32_t> index_buffer;
		std::vector<Vertex> vertex_buffer;
	};
	struct Mesh
	{
		std::vector<SubMesh> meshs;

		static Mesh createPlane();
		static Mesh createLine(float length, glm::vec3 color);
	};
}