#include "SceneManager.h"
#include "Geometry/GltfLoader.h"

namespace ToolEngine
{
	SceneManager::SceneManager(RenderScene& scene, const std::string& scene_json): m_scene(scene)
	{
		loadScene(scene_json);
		
		for (uint32_t i = 0; i < m_game_objects.size(); i++)
		{
			GameObject& go = m_game_objects[i];
			std::vector<Mesh> mesh_list;

			if (go.mesh_path == "plane")
			{
				mesh_list.push_back(Mesh::createPlane());
			}
			else 
			{
				std::string model_path = Path::getInstance().getAssetPath() + go.mesh_path;
				std::unique_ptr<GltfLoader> loader = std::make_unique<GltfLoader>(model_path);
				for (uint32_t loader_index = 0; loader_index < loader->loaded_index_buffer.size(); loader_index++)
				{
					Mesh mesh;
					mesh.index_buffer = loader->loaded_index_buffer[loader_index];
					mesh.vertex_buffer = loader->loaded_vertex_buffer[loader_index];
					mesh_list.push_back(mesh);
				}
			}

			for (uint32_t mesh_index = 0; mesh_index < mesh_list.size(); mesh_index++)
			{
				Transform transform;
				transform.position = glm::vec3(go.position[0], go.position[1], go.position[2]);
				transform.rotation = Quaternion::fromEulerDegreesXYZ(glm::vec3(go.rotation[0], go.rotation[1], go.rotation[2]));
				transform.scale = glm::vec3(go.scale[0], go.scale[1], go.scale[2]);
				auto mat_json = Path::getInstance().readJson(Path::getInstance().getAssetPath() + go.material_path);
				Material material = Material::deserialize(mat_json);

				Bounding bounding;
				bounding.position = glm::vec3(go.bounding_position[0], go.bounding_position[1], go.bounding_position[2]);
				bounding.data = glm::vec3(go.bounding_data[0], go.bounding_data[1], go.bounding_data[2]);
				if (go.bounding_type == 0)
				{
					bounding.type = BoundingType::Box;
				}
				else if (go.bounding_type == 1)
				{
					bounding.type = BoundingType::Sphere;
				}

				m_scene.go_id_list.push_back(go.id);
				std::string name = go.name + std::to_string(mesh_index);
				m_scene.mesh_name_list.push_back(name);
				m_scene.mesh_list.push_back(mesh_list[mesh_index]);
				m_scene.mesh_transform_list.push_back(transform);
				m_scene.material_list.push_back(material);
				m_scene.bounding_list.push_back(bounding);
			}
			
		}
	}
	SceneManager::~SceneManager()
	{
		m_game_objects.clear();
	}
	void SceneManager::loadScene(const std::string& scene_json)
	{
		if (scene_json != m_current_scene)
		{
			m_current_scene = scene_json;
			std::string file_path = Path::getInstance().getAssetPath() + scene_json;
			auto j = Path::getInstance().readJson(file_path);
			deserialize(j);
		}
	}
	void SceneManager::saveScene(const std::string& scene_json)
	{
		std::string file_path = Path::getInstance().getAssetPath() + scene_json;
		auto j = serialize();
		Path::getInstance().saveJson(file_path, j);
	}
	void SceneManager::tick()
	{
	}
	nlohmann::json SceneManager::serialize() const
	{
		nlohmann::json scene;
		std::vector<nlohmann::json> game_objects;
		for (const auto& go : m_game_objects)
		{
			game_objects.push_back(go.serialize());
		}
		scene["game_objects"] = game_objects;

		return scene;
	}
	void SceneManager::deserialize(const nlohmann::json& j)
	{
		m_game_objects.clear();
		auto game_objects = j.at("game_objects").get<std::vector<nlohmann::json>>();
		for (const auto& go : game_objects)
		{
			m_game_objects.push_back(GameObject::deserialize(go));
		}
	}
}
