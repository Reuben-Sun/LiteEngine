#include "CullingResult.h"
#include "Core/Path/Path.h"
#include "Geometry/GltfLoader.h"

namespace ToolEngine
{
	CullingResult::CullingResult(RHIDevice& device, RHIDescriptorSetLayout& layout, RHIDescriptorPool& pool)
		: m_device(device), m_ubo_descriptor_set_layout(layout), m_ubo_descriptor_pool(pool)
	{
	}
	CullingResult::~CullingResult()
	{
	}
	void CullingResult::cull(RenderScene& scene)
	{
		OPTICK_EVENT();
		// TODO: currrent without culling
		for (int entity_index = 0; entity_index < scene.render_entities.size(); entity_index++)
		{
			auto& entity = scene.render_entities[entity_index];
			OPTICK_PUSH("Process Material");
			// material
			for (int material_index = 0; material_index < entity.material_names.size(); material_index++)
			{
				auto& material_name = entity.material_names[material_index];
				auto material_path = Path::getInstance().getAssetPath() + material_name;
				nlohmann::json material_json = Path::getInstance().readJson(material_path);
				Material material = Material::deserialize(material_json);
				// texture
				
				// material descriptor set
				OPTICK_PUSH("Process Material Descriptor Set");
				if (m_material_name_to_descriptor_set.find(material_name) == m_material_name_to_descriptor_set.end())
				{
					// create descriptor set and update uniform buffer
					m_material_name_to_descriptor_set.emplace(material_name,
						std::make_unique<RHIDescriptorSet>(m_device, m_ubo_descriptor_pool, m_ubo_descriptor_set_layout));
					m_material_name_to_descriptor_set[material_name]->updateUniformBuffer(*scene.m_resources->m_global_ubo, 0);

					// update texture
					uint32_t texture_enable = 0;
					for (int j = TEXTURE_MIN_BINDING; j <= TEXTURE_MAX_BINDING; j++)
					{
						// Hack: give each binding point default texture
						m_material_name_to_descriptor_set[material_name]->updateTextureImage(scene.m_resources->m_global_default_texture->m_descriptor, j);
						if (j == 5)
						{
							// the 5 texture is skybox texture
							m_material_name_to_descriptor_set[material_name]->updateTextureImage(scene.m_resources->m_skybox_texture->m_descriptor, j);
						}
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
						m_material_name_to_descriptor_set[material_name]->updateTextureImage(scene.m_resources->m_texture_name_to_image[texture_name]->m_descriptor,
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
				OPTICK_POP();
			}
			OPTICK_POP();
		}
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