#include "RHI/Public/RHICommandBuffer.h"

namespace ToolEngine
{
	RHICommandBuffer::RHICommandBuffer(RHIDevice& device, uint32_t command_buffer_count)
		: m_device(device), m_command_buffer_count(command_buffer_count)
	{
		VkCommandBufferAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.commandPool = m_device.getCommandPool();
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandBufferCount = m_command_buffer_count;

		m_command_buffers.resize(m_command_buffer_count);
		if (vkAllocateCommandBuffers(m_device.getLogicalDevice(), &alloc_info, m_command_buffers.data()) != VK_SUCCESS)
		{
			LOG_ERROR("failed to allocate command buffers!");
		}
		LOG_INFO("Create Command Buffer!");
	}
	RHICommandBuffer::~RHICommandBuffer()
	{
	}
}