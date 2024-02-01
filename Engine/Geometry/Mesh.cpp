#include "Mesh.h"

namespace ToolEngine
{
	Mesh Mesh::createPlane()
	{
		std::vector<uint32_t> plane_index_buffer = { 0, 1, 2, 2, 3, 0 };
		const std::vector<Vertex> plane_vertex_buffer =
		{
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
		};
		Mesh mesh;
		mesh.index_buffer = plane_index_buffer;
		mesh.vertex_buffer = plane_vertex_buffer;
		return mesh;
	}
}