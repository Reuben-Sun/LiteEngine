#pragma once

#include "Marco.h"
#include <json.hpp>

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

		nlohmann::json serialize() const
		{
			nlohmann::json texture_bindings;
			for (const auto& tb : this->texture_bindings)
			{
				texture_bindings.push_back(nlohmann::json{
					{"binding_index", tb.binding_index},
					{"texture_path", tb.texture_path}
				});
			}
			return nlohmann::json{
				{"vertex_shader_path", vertex_shader_path},
				{"frag_shader_path", frag_shader_path},
				{"texture_bindings", texture_bindings}
			};
		}

		static Material deserialize(const nlohmann::json& j)
		{
			Material material;
			material.vertex_shader_path = j.at("vertex_shader_path").get<std::string>();
			material.frag_shader_path = j.at("frag_shader_path").get<std::string>();
			for (const auto& tb : j.at("texture_bindings"))
			{
				material.texture_bindings.push_back(TextureBinding{
					tb.at("binding_index").get<uint32_t>(),
					tb.at("texture_path").get<std::string>()
				});
			}
			return material;
		}
	};
}