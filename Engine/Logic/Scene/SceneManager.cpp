#include "SceneManager.h"
#include "Geometry/GltfLoader.h"

namespace ToolEngine
{
	SceneManager::SceneManager(RenderScene& scene, const std::string& scene_json): m_scene(scene)
	{
		m_logic_scene.loadScene(scene_json);
		auto view = m_logic_scene.scene_context.view<const InfoComponent, 
			const MeshComponent, const MaterialComponent, const TransformComponent, const BoundingComponent>();
		for (auto entity : view)
		{
			auto &info_component = view.get<InfoComponent>(entity);
			m_scene.go_id_list.push_back(info_component.id);
			m_scene.mesh_name_list.push_back(info_component.name);

			auto& mesh_component = view.get<MeshComponent>(entity);
			Mesh mesh;
			if (mesh_component.mesh_path == "plane")
			{
				mesh = Mesh::createPlane();
			}
			else
			{
				std::string model_path = Path::getInstance().getAssetPath() + mesh_component.mesh_path;
				std::unique_ptr<GltfLoader> loader = std::make_unique<GltfLoader>(model_path);
				for (uint32_t loader_index = 0; loader_index < loader->loaded_index_buffer.size(); loader_index++)
				{
					SubMesh sub_mesh;
					sub_mesh.index_buffer = loader->loaded_index_buffer[loader_index];
					sub_mesh.vertex_buffer = loader->loaded_vertex_buffer[loader_index];
					mesh.meshs.push_back(sub_mesh);
				}
			}
			m_scene.mesh_list.push_back(mesh);

			auto &material_component = view.get<MaterialComponent>(entity);
			m_scene.material_name_list.push_back(material_component.material_path);
			auto mat_json = Path::getInstance().readJson(Path::getInstance().getAssetPath() + material_component.material_path);
			Material material = Material::deserialize(mat_json);
			m_scene.name_to_material_map[material_component.material_path] = material;

			auto &transform_component = view.get<TransformComponent>(entity);
			m_scene.mesh_transform_list.push_back(transform_component.transform);

			auto& bounding_component = view.get<BoundingComponent>(entity);
			m_scene.bounding_list.push_back(bounding_component.bounding);
		}
	}
	SceneManager::~SceneManager()
	{
	}
	
	void SceneManager::tick()
	{
	}
}
