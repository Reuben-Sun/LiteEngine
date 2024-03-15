#include "RenderScene.h"
#include "Core/Path/Path.h"
#include "Geometry/GltfLoader.h"

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
	void RenderScene::tick()
	{
		resources->tick(render_entities);
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
		auto ktx_image_path = Path::getInstance().getAssetPath() + "Textures\\Cubemap\\skybox.ktx2";
		//m_skybox_texture = std::make_unique<RHITextureCube>(m_device, skybox_images);
		m_skybox_texture = std::make_unique<RHITextureCube>(m_device, ktx_image_path);
	}
	SceneResources::~SceneResources()
	{
		m_sub_mesh_name_to_vertex_buffer.clear();
		m_sub_mesh_name_to_index_buffer.clear();
		m_model_name_to_sub_model_name.clear();
		m_texture_name_to_image.clear();
	}
	void SceneResources::tick(std::vector<RenderEntity>& entities)
	{
		OPTICK_EVENT();
		for (int entity_index = 0; entity_index < entities.size(); entity_index++)
		{
			auto& mesh_path = entities[entity_index].mesh_path;
			OPTICK_PUSH("Process Mesh");
			if (m_model_name_to_sub_model_name.find(mesh_path) == m_model_name_to_sub_model_name.end())
			{
				if (mesh_path == "plane")
				{
					Mesh plane_mesh = Mesh::createPlane();
					m_sub_mesh_name_to_index_buffer.emplace(mesh_path,
						std::make_unique<RHIIndexBuffer>(m_device, plane_mesh.meshs[0].index_buffer));
					m_sub_mesh_name_to_vertex_buffer.emplace(mesh_path,
						std::make_unique<RHIVertexBuffer>(m_device, plane_mesh.meshs[0].vertex_buffer));
					m_model_name_to_sub_model_name.emplace(mesh_path, std::vector<std::string>{mesh_path});
				}
				else
				{
					std::string model_path = Path::getInstance().getAssetPath() + mesh_path;
					std::unique_ptr<GltfLoader> loader = std::make_unique<GltfLoader>(model_path);
					std::vector<std::string> sub_mesh_names;
					for (int sub_mesh_index = 0; sub_mesh_index < loader->loaded_index_buffer.size(); sub_mesh_index++)
					{
						auto sub_mesh_name = mesh_path + std::to_string(sub_mesh_index);
						m_sub_mesh_name_to_index_buffer.emplace(sub_mesh_name, std::make_unique<RHIIndexBuffer>(m_device, loader->loaded_index_buffer[sub_mesh_index]));
						m_sub_mesh_name_to_vertex_buffer.emplace(sub_mesh_name, std::make_unique<RHIVertexBuffer>(m_device, loader->loaded_vertex_buffer[sub_mesh_index]));
						sub_mesh_names.push_back(sub_mesh_name);
					}
					m_model_name_to_sub_model_name.emplace(mesh_path, sub_mesh_names);
				}
			}
			OPTICK_POP();

			OPTICK_PUSH("Process Material");
			// material
			for (int material_index = 0; material_index < entities[entity_index].material_names.size(); material_index++)
			{
				auto& material_name = entities[entity_index].material_names[material_index];
				auto material_path = Path::getInstance().getAssetPath() + material_name;
				nlohmann::json material_json = Path::getInstance().readJson(material_path);
				Material material = Material::deserialize(material_json);
				// texture
				OPTICK_PUSH("Process Texture");
				for (int j = 0; j < material.texture_bindings.size(); j++)
				{
					std::string texture_name = material.texture_bindings[j].texture_path;
					if (m_texture_name_to_image.find(texture_name) == m_texture_name_to_image.end())
					{
						std::string texture_path = Path::getInstance().getAssetPath() + texture_name;
						m_texture_name_to_image.emplace(texture_name, std::make_unique<RHITextureImage>(m_device, texture_path));
					}
				}
				OPTICK_POP();
				OPTICK_PUSH("Process Push constant");
				if (m_material_name_to_push_constant.find(material_name) == m_material_name_to_push_constant.end())
				{
					PushConstant push_constant;
					push_constant.base_color = { material.base_color[0], material.base_color[1], material.base_color[2] };
					push_constant.emission_color = glm::vec3(0.0f, 0.0f, 0.0f);
					push_constant.metallic = material.metallic;
					push_constant.roughness = material.roughness;
					// without texture use
					m_material_name_to_push_constant.emplace(material_name, push_constant);
				}
				
				OPTICK_POP();
			}
			OPTICK_POP();
		}
	}
	RHIVertexBuffer& SceneResources::getVertexBuffer(const std::string& sub_model_name)
	{
		auto it = m_sub_mesh_name_to_vertex_buffer.find(sub_model_name);
		RHIVertexBuffer& vertex_buffer_ref = *(it->second.get());
		return vertex_buffer_ref;
	}
	RHIIndexBuffer& SceneResources::getIndexBuffer(const std::string& sub_model_name)
	{
		auto it = m_sub_mesh_name_to_index_buffer.find(sub_model_name);
		RHIIndexBuffer& index_buffer_ref = *(it->second.get());
		return index_buffer_ref;
	}
}