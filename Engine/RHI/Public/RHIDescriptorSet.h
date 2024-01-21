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

	class RHIDescriptorSetLayout
	{
	public:
		RHIDescriptorSetLayout(RHIDevice& device);
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
		void updateTextureImage(RHITextureImage& texture_image, uint32_t binding_index);
	private:
		VkDescriptorSet m_descriptor_set{ VK_NULL_HANDLE };
		RHIDevice& m_device;
		RHIDescriptorPool& m_descriptor_pool;
		RHIDescriptorSetLayout& m_descriptor_set_layout;
	};
}