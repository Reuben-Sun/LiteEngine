#pragma once

#include "Marco.h"
#include <json.hpp>

namespace ToolEngine
{
	struct GameObject
	{
		std::string name;
		uint32_t id;
		std::string mesh_path;
		std::string material_path;
		std::vector<float> position;
		std::vector<float> rotation;	// euler angles
		std::vector<float> scale;
		uint32_t bounding_type;		// 0 is box, 1 is sphere
		std::vector<float> bounding_position;
		std::vector<float> bounding_data;

		static uint32_t global_id;

		nlohmann::json serialize() const
		{
			return nlohmann::json{
				{"name", name},
				{"mesh_path", mesh_path},
				{"material_path", material_path},
				{"position", position},
				{"rotation", rotation},
				{"scale", scale},
				{"bounding_type", bounding_type},
				{"bounding_position", bounding_position},
				{"bounding_data", bounding_data}
			};
		}

		static GameObject deserialize(const nlohmann::json& j)
		{
			GameObject go;
			go.name = j.at("name").get<std::string>();
			go.mesh_path = j.at("mesh_path").get<std::string>();
			go.material_path = j.at("material_path").get<std::string>();
			go.position = j.at("position").get<std::vector<float>>();
			go.rotation = j.at("rotation").get<std::vector<float>>();
			go.scale = j.at("scale").get<std::vector<float>>();
			go.bounding_type = j.at("bounding_type").get<uint32_t>();
			go.bounding_position = j.at("bounding_position").get<std::vector<float>>();
			go.bounding_data = j.at("bounding_data").get<std::vector<float>>();
			go.id = global_id++;
			return go;
		}
	};
}