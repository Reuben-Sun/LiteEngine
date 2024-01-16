#include "RHI/Public/RHIDescriptorSet.h"

namespace ToolEngine
{
	RHIDescriptorSetLayout::RHIDescriptorSetLayout(RHIDevice& device, uint32_t binding_index)
		: m_device(device)
	{
		VkDescriptorSetLayoutBinding ubo_layout_binding{};
		ubo_layout_binding.binding = binding_index;
		ubo_layout_binding.descriptorCount = 1;
		ubo_layout_binding.descriptorType = RHIDescriptorTypeMap[RHIDescriptorType::ConstantBuffer];
		ubo_layout_binding.pImmutableSamplers = nullptr;
		ubo_layout_binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		std::vector<VkDescriptorSetLayoutBinding> bindings = { ubo_layout_binding };
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
	RHIDescriptorSet::RHIDescriptorSet(RHIDevice& device, RHIDescriptorPool& descriptor_pool, RHIDescriptorSetLayout& descriptor_set_layout, RHIUniformBuffer& ubo_buffer)
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

		std::vector<VkWriteDescriptorSet> descriptor_writes(1);
		descriptor_writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor_writes[0].dstSet = m_descriptor_set;
		descriptor_writes[0].dstBinding = 0;
		descriptor_writes[0].dstArrayElement = 0;
		descriptor_writes[0].descriptorType = RHIDescriptorTypeMap[RHIDescriptorType::ConstantBuffer];
		descriptor_writes[0].descriptorCount = 1;
		descriptor_writes[0].pBufferInfo = &ubo_buffer.m_descriptor;
		uint32_t descriptor_write_count = static_cast<uint32_t>(descriptor_writes.size());
		vkUpdateDescriptorSets(m_device.getLogicalDevice(), descriptor_write_count, descriptor_writes.data(), 0, nullptr);
	}
	RHIDescriptorSet::~RHIDescriptorSet()
	{
	}
}