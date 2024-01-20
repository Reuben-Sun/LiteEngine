#pragma once

#include "Marco.h"
#include "tiny_gltf.h"
#include "Vertex.h"

namespace ToolEngine
{
	class GltfLoader
	{
	public:
		GltfLoader(std::string file_path);
		~GltfLoader();

		std::vector<uint32_t> loaded_index_buffer;
		std::vector<Vertex> loaded_vertex_buffer;
	private:
		tinygltf::Model m_model;
		tinygltf::TinyGLTF m_context;
	};
}