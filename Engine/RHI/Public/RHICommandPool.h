#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>

namespace ToolEngine
{
	class RHICommandPool
	{
	public:
		RHICommandPool(VkDevice device, uint32_t queue_family_index);
		~RHICommandPool();

	VkCommandPool getHandle() const { return m_command_pool; }
		
	private:
		VkCommandPool m_command_pool;
		VkDevice m_device;
	};
}