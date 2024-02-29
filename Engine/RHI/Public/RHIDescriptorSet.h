#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"
#include "RHIUniformBuffer.h"
#include "RHIDescriptorPool.h"
#include "RHI/Public/RHITextureImage.h"

namespace ToolEngine
{
	enum class RHIDescriptorType
	{
		Sampler,
		TextureSRV,
		TextureUAV,
		ConstantBuffer,
		StructuredBuffer
	};

	static std::unordered_map<RHIDescriptorType, VkDescriptorType> RHIDescriptorTypeMap =
	{
		{ RHIDescriptorType::Sampler, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER },
		{ RHIDescriptorType::TextureSRV, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE },
		{ RHIDescriptorType::TextureUAV, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE },
		{ RHIDescriptorType::ConstantBuffer, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER },
		{ RHIDescriptorType::StructuredBuffer, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER }
	};

	static std::unordered_map<RHIDescriptorType, VkShaderStageFlags> RHIDescriptorShaderStageMap =
	{
		{ RHIDescriptorType::Sampler, VK_SHADER_STAGE_FRAGMENT_BIT },
		{ RHIDescriptorType::ConstantBuffer, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
		{ RHIDescriptorType::TextureSRV, VK_SHADER_STAGE_FRAGMENT_BIT}
	};

	class RHIDescriptorSetLayout
	{
	public:
		RHIDescriptorSetLayout(RHIDevice& device, std::vector<RHIDescriptorType>& layout_descriptor);	// TODO: generate descriptor set by shader meta file
		~RHIDescriptorSetLayout();

		VkDescriptorSetLayout getHandle() const { return m_layout; }
	private:
		VkDescriptorSetLayout m_layout{ VK_NULL_HANDLE };
		RHIDevice& m_device;
	};

	class RHIDescriptorSet
	{
	public:
		RHIDescriptorSet(RHIDevice& device, RHIDescriptorPool& descriptor_pool, RHIDescriptorSetLayout& descriptor_set_layout);
		~RHIDescriptorSet();

		VkDescriptorSet getHandle() const { return m_descriptor_set; }
		void updateUniformBuffer(RHIUniformBuffer& ubo_buffer, uint32_t binding_index);
		void updateTextureImage(VkDescriptorImageInfo image_info, uint32_t binding_index, RHIDescriptorType texture_type = RHIDescriptorType::TextureSRV);
	private:
		VkDescriptorSet m_descriptor_set{ VK_NULL_HANDLE };
		RHIDevice& m_device;
		RHIDescriptorPool& m_descriptor_pool;
		RHIDescriptorSetLayout& m_descriptor_set_layout;
	};
}