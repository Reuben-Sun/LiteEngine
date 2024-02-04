#include "SceneManager.h"
#include "Geometry/GltfLoader.h"

namespace ToolEngine
{
	SceneManager::SceneManager(const std::string& scene_json)
	{
		loadScene(scene_json);
		Camera camera;
		camera.transform.position = glm::vec3(3.0f, 3.0f, 3.0f);
		camera.transform.rotation = Quaternion::fromEulerDegreesXYZ(glm::vec3(-63.8f, 0.0f, -133.4f));
		camera.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		camera.fov = glm::radians(45.0f);
		camera.near_plane = 0.1f;
		camera.far_plane = 10.0f;
		camera.view_size = 10;
		m_render_scene.camera = camera;
		for (uint32_t i = 0; i < m_game_objects.size(); i++)
		{
			GameObject& go = m_game_objects[i];
			Mesh mesh;
			if (go.mesh_path == "plane")
			{
				mesh = Mesh::createPlane();
			}
			else 
			{
				std::string model_path = Path::getInstance().getAssetPath() + go.mesh_path;
				std::unique_ptr<GltfLoader> loader = std::make_unique<GltfLoader>(model_path);
				mesh.index_buffer = loader->loaded_index_buffer;
				mesh.vertex_buffer = loader->loaded_vertex_buffer;
			}
			
			Transform transform;
			transform.position = glm::vec3(go.position[0], go.position[1], go.position[2]);
			transform.rotation = Quaternion::fromEulerDegreesXYZ(glm::vec3(go.rotation[0], go.rotation[1], go.rotation[2]));
			transform.scale = glm::vec3(go.scale[0], go.scale[1], go.scale[2]);
			auto mat_json = Path::getInstance().readJson(Path::getInstance().getAssetPath() + go.material_path);
			Material material = Material::deserialize(mat_json);

			m_render_scene.mesh_name_list.push_back(go.name);
			m_render_scene.mesh_list.push_back(mesh);
			m_render_scene.mesh_transform_list.push_back(transform);
			m_render_scene.material_list.push_back(material);
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
