#include "RenderScene.h"
#include "Core/Path/Path.h"

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
	SceneResources::SceneResources(RHIDevice& device): m_device(device)
	{
		m_dir_light.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		m_dir_light.direction = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		m_dir_light.intensity = 1.0f;
		m_dir_light.position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		m_global_ubo = std::make_unique<RHIUniformBuffer>(m_device, sizeof(GlobalUBO));
		auto global_texture_path = Path::getInstance().getAssetPath() + "Textures\\1024.png";
		m_global_default_texture = std::make_unique<RHITextureImage>(m_device, global_texture_path);

		std::vector<std::string> skybox_images;
		auto image_path = Path::getInstance().getAssetPath() + "Textures\\Cubemap\\";
		skybox_images.push_back(image_path + "nx.png");		// +x 
		skybox_images.push_back(image_path + "px.png");		// -x
		skybox_images.push_back(image_path + "ny.png");		// +y
		skybox_images.push_back(image_path + "py.png");		// -y
		skybox_images.push_back(image_path + "nz.png");		// +z
		skybox_images.push_back(image_path + "pz.png");		// -z

		m_skybox_texture = std::make_unique<RHITextureCube>(m_device, skybox_images);
	}
	SceneResources::~SceneResources()
	{
	}
}