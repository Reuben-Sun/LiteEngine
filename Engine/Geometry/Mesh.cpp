#include "Mesh.h"

namespace ToolEngine
{
	Mesh Mesh::createPlane()
	{
		std::vector<uint32_t> plane_index_buffer = { 0, 1, 2, 2, 3, 0 };
		const std::vector<Vertex> plane_vertex_buffer =
		{
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.0f}}
		};
		Mesh mesh;
		SubMesh sub_mesh;
		sub_mesh.index_buffer = plane_index_buffer;
		sub_mesh.vertex_buffer = plane_vertex_buffer;
		mesh.meshs.push_back(sub_mesh);
		return mesh;
	}
	Mesh Mesh::createLine(float length, glm::vec3 color)
	{
		std::vector<uint32_t> plane_index_buffer = { 0, 1, 2, 2, 3, 0 };
		const std::vector<Vertex> plane_vertex_buffer =
		{
			{{-length, -0.001f, 0.0f}, color, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.0f}},
			{{length, -0.001f, 0.0f}, color, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.0f}},
			{{length, 0.001f, 0.0f}, color, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.0f}},
			{{-length, 0.001f, 0.0f}, color, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.0f}}
		};
		Mesh mesh;
		SubMesh sub_mesh;
		sub_mesh.index_buffer = plane_index_buffer;
		sub_mesh.vertex_buffer = plane_vertex_buffer;
		mesh.meshs.push_back(sub_mesh);
		return mesh;
	}
}