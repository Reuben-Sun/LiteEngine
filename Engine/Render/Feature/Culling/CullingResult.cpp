#include "CullingResult.h"
#include "Core/Path/Path.h"
#include "Geometry/GltfLoader.h"

namespace ToolEngine
{
	CullingResult::CullingResult(RHIDevice& device, RHIDescriptorSetLayout& layout, RHIDescriptorPool& pool)
		: m_device(device), m_ubo_descriptor_set_layout(layout), m_ubo_descriptor_pool(pool)
	{
		m_dir_light.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		m_dir_light.direction = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
		m_dir_light.intensity = 1.0f;
		m_dir_light.position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		m_global_ubo = std::make_unique<RHIUniformBuffer>(m_device, sizeof(GlobalUBO));
		auto global_texture_path = Path::getInstance().getAssetPath() + "\\default.jpg";
		m_global_default_texture = std::make_unique<RHITextureImage>(m_device, global_texture_path);
	}
	CullingResult::~CullingResult()
	{
		m_sub_mesh_name_to_index_buffer.clear();
		m_sub_mesh_name_to_vertex_buffer.clear();
		m_texture_name_to_image.clear();
	}
	void CullingResult::cull(RenderScene& scene)
	{
		// TODO: currrent without culling
		for (int i = 0; i < scene.render_entities.size(); i++)
		{
			auto& entity = scene.render_entities[i];
			auto& mesh_name = entity.mesh_name;
			// mesh
			if(m_sub_mesh_name_to_index_buffer.find(mesh_name) == m_sub_mesh_name_to_index_buffer.end())
			{
				if (mesh_name == "plane")
				{
					Mesh plane_mesh = Mesh::createPlane();
					m_sub_mesh_name_to_index_buffer.emplace(mesh_name, 
						std::make_unique<RHIIndexBuffer>(m_device, plane_mesh.meshs[0].index_buffer));
					m_sub_mesh_name_to_vertex_buffer.emplace(mesh_name, 
						std::make_unique<RHIVertexBuffer>(m_device, plane_mesh.meshs[0].vertex_buffer));
					m_model_name_to_sub_model_name.emplace(mesh_name, std::vector<std::string>{mesh_name});
				}
				else 
				{
					std::string model_path = Path::getInstance().getAssetPath() + mesh_name + ".gltf";
					std::unique_ptr<GltfLoader> loader = std::make_unique<GltfLoader>(model_path);
					std::vector<std::string> sub_mesh_names;
					for (int sub_mesh_index = 0; sub_mesh_index < loader->loaded_index_buffer.size(); sub_mesh_index++)
					{
						auto sub_mesh_name = mesh_name + std::to_string(sub_mesh_index);
						m_sub_mesh_name_to_index_buffer.emplace(sub_mesh_name, std::make_unique<RHIIndexBuffer>(m_device, loader->loaded_index_buffer[sub_mesh_index]));
						m_sub_mesh_name_to_vertex_buffer.emplace(sub_mesh_name, std::make_unique<RHIVertexBuffer>(m_device, loader->loaded_vertex_buffer[sub_mesh_index]));
						sub_mesh_names.push_back(sub_mesh_name);
					}
					m_model_name_to_sub_model_name.emplace(mesh_name, sub_mesh_names);
				}
				
			}
			// material
			for (int i = 0; i < entity.material_names.size(); i++)
			{
				auto& material_name = entity.material_names[i];
				auto material_path = Path::getInstance().getAssetPath() + material_name;
				nlohmann::json material_json = Path::getInstance().readJson(material_path);
				Material material = Material::deserialize(material_json);
				// texture
				for (int j = 0; j < material.texture_bindings.size(); j++)
				{
					std::string texture_name = material.texture_bindings[j].texture_path;
					if (m_texture_name_to_image.find(texture_name) == m_texture_name_to_image.end())
					{
						std::string texture_path = Path::getInstance().getAssetPath() + texture_name;
						m_texture_name_to_image.emplace(texture_name, std::make_unique<RHITextureImage>(m_device, texture_path));
					}
				}
				// material descriptor set
				if (m_material_name_to_descriptor_set.find(material_name) == m_material_name_to_descriptor_set.end())
				{
					// create descriptor set and update uniform buffer
					m_material_name_to_descriptor_set.emplace(material_name,
						std::make_unique<RHIDescriptorSet>(m_device, m_ubo_descriptor_pool, m_ubo_descriptor_set_layout));
					m_material_name_to_descriptor_set[material_name]->updateUniformBuffer(*m_global_ubo, 0);

					// update texture
					uint32_t texture_enable = 0;
					for (int j = TEXTURE_MIN_BINDING; j <= TEXTURE_MAX_BINDING; j++)
					{
						// Hack: give each binding point default texture
						m_material_name_to_descriptor_set[material_name]->updateTextureImage(m_global_default_texture->m_descriptor, j);
					}
					for (int j = 0; j < material.texture_bindings.size(); j++)
					{
						std::string texture_name = material.texture_bindings[j].texture_path;
						RHIDescriptorType binding_type;
						if (material.texture_bindings[j].binding_type == 0)
						{
							binding_type = RHIDescriptorType::Sampler;
						}
						else if (material.texture_bindings[j].binding_type = 1)
						{
							binding_type = RHIDescriptorType::TextureSRV;
						}
						m_material_name_to_descriptor_set[material_name]->updateTextureImage(m_texture_name_to_image[texture_name]->m_descriptor,
							material.texture_bindings[j].binding_index, binding_type);

						uint32_t enable_byte = 1 << material.texture_bindings[j].binding_index;
						texture_enable |= enable_byte;
					}
					// push constant
					PushConstant push_constant;
					push_constant.base_color = glm::vec3(1.0f, 1.0f, 1.0f);
					push_constant.emission_color = glm::vec3(0.0f, 0.0f, 0.0f);
					push_constant.metallic = 1.0f;
					push_constant.roughness = 1.0f;
					push_constant.texture_enable = texture_enable;
					m_material_name_to_push_constant.emplace(material_name, push_constant);
				}
			}
		}
	}
	RHIVertexBuffer& CullingResult::getVertexBuffer(const std::string& model_name)
	{
		auto it = m_sub_mesh_name_to_vertex_buffer.find(model_name);
		RHIVertexBuffer& vertex_buffer_ref = *(it->second.get());
		return vertex_buffer_ref;
	}
	RHIIndexBuffer& CullingResult::getIndexBuffer(const std::string& model_name)
	{
		auto it = m_sub_mesh_name_to_index_buffer.find(model_name);
		RHIIndexBuffer& index_buffer_ref = *(it->second.get());
		return index_buffer_ref;
	}
	
	RHIDescriptorSet& CullingResult::getDescriptorSet(const std::string& material_name)
	{
		auto it = m_material_name_to_descriptor_set.find(material_name);
		RHIDescriptorSet& descriptor_set_ref = *(it->second.get());
		return descriptor_set_ref;
	}
	PushConstant CullingResult::getPushConstant(const std::string& material_name)
	{
		return m_material_name_to_push_constant[material_name];;
	}
}