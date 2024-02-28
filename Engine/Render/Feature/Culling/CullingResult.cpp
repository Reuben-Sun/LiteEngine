#include "CullingResult.h"
#include "Core/Path/Path.h"

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
	}
	CullingResult::~CullingResult()
	{
		m_model_name_to_index_buffer.clear();
		m_model_name_to_vertex_buffer.clear();
		m_texture_name_to_image.clear();
	}
	void CullingResult::cull(RenderScene& scene)
	{
		// TODO: currrent without culling
		
		// process mesh
		for (int i = 0; i < scene.mesh_name_list.size(); i++)
		{
			if(m_model_name_to_index_buffer.find(scene.mesh_name_list[i]) == m_model_name_to_index_buffer.end())
			{
				std::string model_name = scene.mesh_name_list[i];
				m_model_name_to_index_buffer.emplace(model_name, std::make_unique<RHIIndexBuffer>(m_device, scene.mesh_list[i].index_buffer));
				m_model_name_to_vertex_buffer.emplace(model_name, std::make_unique<RHIVertexBuffer>(m_device, scene.mesh_list[i].vertex_buffer));
				m_model_name_to_material_name.emplace(model_name, scene.material_name_list[i]);
			}
		}
		// each material has a descriptor set
		for (auto item : scene.name_to_material_map)
		{
			auto name = item.first;
			auto material = item.second;
			// create descriptor set and update uniform buffer
			m_material_name_to_descriptor_set.emplace(name,
				std::make_unique<RHIDescriptorSet>(m_device, m_ubo_descriptor_pool, m_ubo_descriptor_set_layout));
			m_material_name_to_descriptor_set[name]->updateUniformBuffer(*m_global_ubo, 0);
			// load texture
			for (int j = 0; j < material.texture_bindings.size(); j++)
			{
				std::string texture_name = material.texture_bindings[j].texture_path;
				if (m_texture_name_to_image.find(texture_name) == m_texture_name_to_image.end())
				{
					std::string texture_path = Path::getInstance().getAssetPath() + texture_name;
					m_texture_name_to_image.emplace(texture_name, std::make_unique<RHITextureImage>(m_device, texture_path));
				}
			}
			// update texture
			uint32_t texture_enable = 0;
			for (int j = 0; j < material.texture_bindings.size(); j++)
			{
				std::string texture_name = material.texture_bindings[j].texture_path;
				m_material_name_to_descriptor_set[name]->updateTextureImage(m_texture_name_to_image[texture_name]->m_descriptor, material.texture_bindings[j].binding_index);
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
			m_material_name_to_push_constant.emplace(name, push_constant);
		}
	}
	RHIVertexBuffer& CullingResult::getVertexBuffer(const std::string& model_name)
	{
		auto it = m_model_name_to_vertex_buffer.find(model_name);
		RHIVertexBuffer& vertex_buffer_ref = *(it->second.get());
		return vertex_buffer_ref;
	}
	RHIIndexBuffer& CullingResult::getIndexBuffer(const std::string& model_name)
	{
		auto it = m_model_name_to_index_buffer.find(model_name);
		RHIIndexBuffer& index_buffer_ref = *(it->second.get());
		return index_buffer_ref;
	}
	
	RHIDescriptorSet& CullingResult::getDescriptorSet(const std::string& model_name)
	{
		auto material_name = m_model_name_to_material_name[model_name];
		auto it = m_material_name_to_descriptor_set.find(material_name);
		RHIDescriptorSet& descriptor_set_ref = *(it->second.get());
		return descriptor_set_ref;
	}
	PushConstant CullingResult::getPushConstant(const std::string& model_name)
	{
		std::string material_name = m_model_name_to_material_name[model_name];
		return m_material_name_to_push_constant[material_name];;
	}
}