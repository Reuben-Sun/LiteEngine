#pragma once

#include "Marco.h"
#include <json.hpp>

namespace ToolEngine
{
	struct TextureBinding
	{
		uint32_t binding_index;
		uint32_t binding_type;
		std::string texture_path;
	};
	struct Material
	{
		// TODO: use material to select shader
		std::string vertex_shader_path;
		std::string frag_shader_path;
		std::vector<TextureBinding> texture_bindings;	
		float metallic;
		float roughness;
		std::vector<float> base_color;
		std::vector<float> emission;

		nlohmann::json serialize() const
		{
			nlohmann::json texture_bindings;
			for (const auto& tb : this->texture_bindings)
			{
				texture_bindings.push_back(nlohmann::json{
					{"binding_index", tb.binding_index},
					{"binding_type", tb.binding_type},
					{"texture_path", tb.texture_path}
				});
			}
			return nlohmann::json{
				{"vertex_shader_path", vertex_shader_path},
				{"frag_shader_path", frag_shader_path},
				{"texture_bindings", texture_bindings},
				{"metallic", metallic},
				{"roughness", roughness},
				{"base_color", base_color},
				{"emission", emission}
			};
		}

		static Material deserialize(const nlohmann::json& j)
		{
			Material material;
			material.vertex_shader_path = j.at("vertex_shader_path").get<std::string>();
			material.frag_shader_path = j.at("frag_shader_path").get<std::string>();
			material.metallic = j.at("metallic").get<float>();
			material.roughness = j.at("roughness").get<float>();
			material.base_color = j.at("base_color").get<std::vector<float>>();
			material.emission = j.at("emission").get<std::vector<float>>();
			for (const auto& tb : j.at("texture_bindings"))
			{
				material.texture_bindings.push_back(TextureBinding{
					tb.at("binding_index").get<uint32_t>(),
					tb.at("binding_type").get<uint32_t>(),
					tb.at("texture_path").get<std::string>()
				});
			}
			return material;
		}
	};
}