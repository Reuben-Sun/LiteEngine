#include "RenderScene.h"

namespace ToolEngine
{
	void RenderScene::init(LogicScene& logic_scene)
	{
		OPTICK_EVENT();
		render_entities.clear();
		auto view = logic_scene.scene_context.view<const InfoComponent,
			const MeshComponent, const MaterialComponent, const TransformComponent, const BoundingComponent>();
		for (auto entity : view)
		{
			auto& info_component = view.get<InfoComponent>(entity);
			auto& mesh_component = view.get<MeshComponent>(entity);
			auto& material_component = view.get<MaterialComponent>(entity);
			auto& transform_component = view.get<TransformComponent>(entity);
			auto& bounding_component = view.get<BoundingComponent>(entity);

			RenderEntity render_entity;
			render_entity.go_id = info_component.id;
			render_entity.mesh_name = info_component.name;
			render_entity.mesh_path = mesh_component.mesh_path;
			render_entity.material_names = material_component.material_paths;
			render_entity.transform = transform_component.transform;
			render_entity.bounding = bounding_component.bounding;
			render_entities.push_back(render_entity);
		}
		auto camera_view = logic_scene.scene_context.view<const CameraComponent>();
		camera = camera_view.get<CameraComponent>(camera_view.front()).camera;
	}
}