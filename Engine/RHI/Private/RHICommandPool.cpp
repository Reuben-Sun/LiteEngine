#include "RHI/Public/RHICommandPool.h"

namespace ToolEngine
{
	RHICommandPool::RHICommandPool(VkDevice device, uint32_t queue_family_index): m_device(device)
	{
		VkCommandPoolCreateInfo pool_info{};
		pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		pool_info.queueFamilyIndex = queue_family_index;

		if (vkCreateCommandPool(m_device, &pool_info, nullptr, &m_command_pool) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create command pool!");
		}
		LOG_INFO("Create command pool!");
	}
	RHICommandPool::~RHICommandPool()
	{
		if (m_command_pool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(m_device, m_command_pool, nullptr);
		}
	}
}