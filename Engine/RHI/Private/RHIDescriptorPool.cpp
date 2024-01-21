#include "RHI/Public/RHIDescriptorPool.h"
#include <RHI/Public/RHIDescriptorSet.h>

namespace ToolEngine
{
	RHIDescriptorPool::RHIDescriptorPool(RHIDevice& device)
		: m_device(device)
	{
		std::vector<VkDescriptorPoolSize> pool_size(2);
		// ubo
		pool_size[0].type = RHIDescriptorTypeMap[RHIDescriptorType::ConstantBuffer];
		pool_size[0].descriptorCount = 1000;
		// texture image
		pool_size[1].type = RHIDescriptorTypeMap[RHIDescriptorType::Sampler];
		pool_size[1].descriptorCount = 1000;

		VkDescriptorPoolCreateInfo pool_create_info{};
		pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_create_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_create_info.poolSizeCount = pool_size.size();
		pool_create_info.pPoolSizes = pool_size.data();
		pool_create_info.maxSets = 10;

		if (vkCreateDescriptorPool(m_device.getLogicalDevice(), &pool_create_info, nullptr, &m_descriptor_pool) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create descriptor pool!");
		}
		LOG_INFO("Create Descriptor Pool!");
	}

	RHIDescriptorPool::~RHIDescriptorPool()
	{
		vkDestroyDescriptorPool(m_device.getLogicalDevice(), m_descriptor_pool, nullptr);
	}
}