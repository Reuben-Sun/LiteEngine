#include "LogicScene.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
	void LogicScene::loadScene(const std::string& path)
	{
		std::string asset_path = Path::getInstance().getAssetPath();
		std::string file_path = asset_path + path;
		auto j = Path::getInstance().readJson(file_path);
		auto game_objects = j.at("game_objects").get<std::vector<nlohmann::json>>();
		for (auto& go : game_objects)
		{
			const auto entity = scene_context.create();

			InfoComponent info_component;
			info_component.name = go.at("name").get<std::string>();
			info_component.id = global_id++;

			MeshComponent mesh_component;
			mesh_component.mesh_path = asset_path + go.at("mesh_path").get<std::string>();

			MaterialComponent material_component;
			material_component.material_path = asset_path + go.at("material_path").get<std::string>();

			TransformComponent transform_component;
			Transform transform;
			auto position = go.at("position").get<std::vector<float>>();
			auto rotation = go.at("rotation").get<std::vector<float>>();
			auto scale = go.at("scale").get<std::vector<float>>();
			transform.position = glm::vec3(position[0], position[1], position[2]);
			transform.rotation = Quaternion::fromEulerDegreesXYZ(glm::vec3(rotation[0], rotation[1], rotation[2]));
			transform.scale = glm::vec3(scale[0], scale[1], scale[2]);
			transform_component.transform = transform;

			BoundingComponent bounding_component;
			Bounding bounding;
			auto bounding_position = go.at("bounding_position").get<std::vector<float>>();
			auto bounding_data = go.at("bounding_data").get<std::vector<float>>();
			auto bounding_type = go.at("bounding_type").get<uint32_t>();
			bounding.position = glm::vec3(bounding_position[0], bounding_position[1], bounding_position[2]);
			bounding.data = glm::vec3(bounding_data[0], bounding_data[1], bounding_data[2]);
			if (bounding_type == 0)
			{
				bounding.type = BoundingType::Box;
			}
			else if (bounding_type == 1)
			{
				bounding.type = BoundingType::Sphere;
			}
			bounding_component.bounding = bounding;

			scene_context.emplace<InfoComponent>(entity, info_component);
			scene_context.emplace<MeshComponent>(entity, mesh_component);
			scene_context.emplace<MaterialComponent>(entity, material_component);
			scene_context.emplace<TransformComponent>(entity, transform_component);
			scene_context.emplace<BoundingComponent>(entity, bounding_component);
		}
	}
}