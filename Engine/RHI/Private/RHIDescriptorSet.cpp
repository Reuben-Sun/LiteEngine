#include "RHI/Public/RHIDescriptorSet.h"

namespace ToolEngine
{
	RHIDescriptorSetLayout::RHIDescriptorSetLayout(RHIDevice& device, std::vector<RHIDescriptorType>& layout_descriptor)
		: m_device(device)
	{
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		for (uint32_t i = 0; i < layout_descriptor.size(); i++)
		{
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = i;
			binding.descriptorCount = 1;
			binding.descriptorType = RHIDescriptorTypeMap[layout_descriptor[i]];
			binding.pImmutableSamplers = nullptr;
			binding.stageFlags = RHIDescriptorShaderStageMap[layout_descriptor[i]];
			bindings.push_back(binding);
		}

		VkDescriptorSetLayoutCreateInfo layout_info{};
		layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
		layout_info.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(m_device.getLogicalDevice(), &layout_info, nullptr, &m_layout) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create descriptor set layout!");
		}
		LOG_INFO("Create Descriptor Set Layout!");
	}
	RHIDescriptorSetLayout::~RHIDescriptorSetLayout()
	{
		if (m_layout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(m_device.getLogicalDevice(), m_layout, nullptr);
		}
	}
	RHIDescriptorSet::RHIDescriptorSet(RHIDevice& device, RHIDescriptorPool& descriptor_pool, RHIDescriptorSetLayout& descriptor_set_layout)
		: m_device(device), m_descriptor_pool(descriptor_pool), m_descriptor_set_layout(descriptor_set_layout)
	{
		std::vector<VkDescriptorSetLayout> layouts = { m_descriptor_set_layout.getHandle() };
		VkDescriptorSetAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorPool = m_descriptor_pool.getHandle();
		alloc_info.descriptorSetCount = 1;
		alloc_info.pSetLayouts = layouts.data();

		if (vkAllocateDescriptorSets(m_device.getLogicalDevice(), &alloc_info, &m_descriptor_set) != VK_SUCCESS)
		{
			LOG_ERROR("failed to allocate descriptor sets!");
		}
	}
	RHIDescriptorSet::~RHIDescriptorSet()
	{
	}
	void RHIDescriptorSet::updateUniformBuffer(RHIUniformBuffer& ubo_buffer, uint32_t binding_index)
	{
		std::vector<VkWriteDescriptorSet> descriptor_writes(1);
		descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor_writes[0].dstSet = m_descriptor_set;
		descriptor_writes[0].dstBinding = binding_index;
		descriptor_writes[0].dstArrayElement = 0;
		descriptor_writes[0].descriptorType = RHIDescriptorTypeMap[RHIDescriptorType::ConstantBuffer];
		descriptor_writes[0].descriptorCount = 1;
		descriptor_writes[0].pBufferInfo = &ubo_buffer.m_descriptor;
		uint32_t descriptor_write_count = static_cast<uint32_t>(descriptor_writes.size());
		vkUpdateDescriptorSets(m_device.getLogicalDevice(), descriptor_write_count, descriptor_writes.data(), 0, nullptr);
	}
	void RHIDescriptorSet::updateTextureImage(VkDescriptorImageInfo image_info, uint32_t binding_index)
	{
		std::vector<VkWriteDescriptorSet> descriptor_writes(1);
		descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor_writes[0].dstSet = m_descriptor_set;
		descriptor_writes[0].dstBinding = binding_index;
		descriptor_writes[0].dstArrayElement = 0;
		descriptor_writes[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor_writes[0].descriptorCount = 1;
		descriptor_writes[0].pImageInfo = &image_info;
		uint32_t descriptor_write_count = static_cast<uint32_t>(descriptor_writes.size());
		vkUpdateDescriptorSets(m_device.getLogicalDevice(), descriptor_write_count, descriptor_writes.data(), 0, nullptr);
	}
}