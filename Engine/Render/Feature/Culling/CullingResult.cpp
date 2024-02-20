#include "CullingResult.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
	CullingResult::CullingResult(RHIDevice& device, RHIDescriptorSetLayout& layout, RHIDescriptorPool& pool)
		: m_device(device), m_ubo_descriptor_set_layout(layout), m_ubo_descriptor_pool(pool)
	{
	}
	CullingResult::~CullingResult()
	{
		m_model_name_to_index_buffer.clear();
		m_model_name_to_vertex_buffer.clear();
		m_model_name_to_uniform_buffer.clear();
		m_texture_name_to_image.clear();
	}
	void CullingResult::cull(RenderScene& scene)
	{
		// TODO: currrent without culling
		// preprocess image
		for (int i = 0; i < scene.material_list.size(); i++)
		{
			auto& material = scene.material_list[i];
			for (int j = 0; j < material.texture_bindings.size(); j++)
			{
				std::string texture_name = material.texture_bindings[j].texture_path;
				if (m_texture_name_to_image.find(texture_name) == m_texture_name_to_image.end())
				{
					std::string texture_path = Path::getInstance().getAssetPath() + texture_name;
					m_texture_name_to_image.emplace(texture_name, std::make_unique<RHITextureImage>(m_device, texture_path));
				}
			}
		}
		// process mesh and ubo
		for (int i = 0; i < scene.mesh_name_list.size(); i++)
		{
			if(m_model_name_to_index_buffer.find(scene.mesh_name_list[i]) == m_model_name_to_index_buffer.end())
			{
				std::string model_name = scene.mesh_name_list[i];
				m_model_name_to_index_buffer.emplace(model_name, std::make_unique<RHIIndexBuffer>(m_device, scene.mesh_list[i].index_buffer));
				m_model_name_to_vertex_buffer.emplace(model_name, std::make_unique<RHIVertexBuffer>(m_device, scene.mesh_list[i].vertex_buffer));
				// ubo
				m_model_name_to_uniform_buffer.emplace(model_name, std::make_unique<RHIUniformBuffer>(m_device, sizeof(GlobalUBO)));
				m_model_name_to_ubo_descriptor_set.emplace(model_name,
					std::make_unique<RHIDescriptorSet>(m_device, m_ubo_descriptor_pool, m_ubo_descriptor_set_layout));
				m_model_name_to_ubo_descriptor_set[model_name]->updateUniformBuffer(*m_model_name_to_uniform_buffer[model_name], 0);
				for (int j = 0; j < scene.material_list[i].texture_bindings.size(); j++)
				{
					std::string texture_name = scene.material_list[i].texture_bindings[j].texture_path;
					m_model_name_to_ubo_descriptor_set[model_name]->updateTextureImage(m_texture_name_to_image[texture_name]->m_descriptor, scene.material_list[i].texture_bindings[j].binding_index);
				}
			}
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
	RHIUniformBuffer& CullingResult::getUniformBuffer(const std::string& model_name)
	{
		auto it = m_model_name_to_uniform_buffer.find(model_name);
		RHIUniformBuffer& uniform_buffer_ref = *(it->second.get());
		return uniform_buffer_ref;
	}
	RHIDescriptorSet& CullingResult::getDescriptorSet(const std::string& model_name)
	{
		auto it = m_model_name_to_ubo_descriptor_set.find(model_name);
		RHIDescriptorSet& descriptor_set_ref = *(it->second.get());
		return descriptor_set_ref;
	}
	RHITextureImage& CullingResult::getTextureImage(const std::string& texture_name)
	{
		auto it = m_texture_name_to_image.find(texture_name);
		RHITextureImage& texture_image_ref = *(it->second.get());
		return texture_image_ref;
	}
}