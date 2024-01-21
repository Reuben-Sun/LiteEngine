#pragma once

#include "Marco.h"

namespace ToolEngine
{
	struct TextureBinding
	{
		uint32_t binding_index;
		std::string texture_path;
	};
	struct Material
	{
		// TODO: use material to select shader
		std::string vertex_shader_path;
		std::string frag_shader_path;
		std::vector<TextureBinding> texture_bindings;	// current only use this
	};
}