#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHICommandBuffer
	{
	public:
		RHICommandBuffer(RHIDevice& device, uint32_t command_buffer_count);
		~RHICommandBuffer();

		VkCommandBuffer getHandle(uint32_t index) const { return m_command_buffers[index]; }
		uint32_t getCommandBufferCount() const { return m_command_buffer_count; }
		
	private:
		RHIDevice& m_device;
		std::vector<VkCommandBuffer> m_command_buffers;
		uint32_t m_command_buffer_count;
	};
}