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
			auto& mesh_component = view.get<MeshComponent>(entity);
			auto &material_component = view.get<MaterialComponent>(entity);
			auto &transform_component = view.get<TransformComponent>(entity);
			auto& bounding_component = view.get<BoundingComponent>(entity);

			RenderEntity render_entity;
			render_entity.go_id = info_component.id;
			render_entity.mesh_name = info_component.name;
			render_entity.material_names = material_component.material_paths;
			render_entity.transform = transform_component.transform;
			render_entity.bounding = bounding_component.bounding;
			m_scene.render_entities.push_back(render_entity);
		}
	}
	SceneManager::~SceneManager()
	{
	}
	
	void SceneManager::tick()
	{
		OPTICK_EVENT();
	}
}
