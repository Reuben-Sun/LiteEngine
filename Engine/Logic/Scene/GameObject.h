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

		static uint32_t global_id;

		nlohmann::json serialize() const
		{
			return nlohmann::json{
				{"name", name},
				{"mesh_path", mesh_path},
				{"material_path", material_path},
				{"position", position},
				{"rotation", rotation},
				{"scale", scale}
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
			go.id = global_id++;
			return go;
		}
	};
}