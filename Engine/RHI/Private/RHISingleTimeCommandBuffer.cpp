#include "RHI/Public/RHISingleTimeCommandBuffer.h"

namespace ToolEngine
{
	RHISingleTimeCommandBuffer::RHISingleTimeCommandBuffer(RHIDevice& device): m_device(device)
	{
		VkCommandBufferAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandPool = m_device.getCommandPool();
		alloc_info.commandBufferCount = 1;

		vkAllocateCommandBuffers(m_device.getLogicalDevice(), &alloc_info, &m_command_buffer);

		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(m_command_buffer, &begin_info);
	}
	RHISingleTimeCommandBuffer::~RHISingleTimeCommandBuffer()
	{
		vkEndCommandBuffer(m_command_buffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_command_buffer;

		vkQueueSubmit(m_device.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_device.getGraphicsQueue());

		vkFreeCommandBuffers(m_device.getLogicalDevice(), m_device.getCommandPool(), 1, &m_command_buffer);
	}
	void RHISingleTimeCommandBuffer::pipelineBarrier(VkPipelineStageFlags source_stage, VkPipelineStageFlags destination_stage, VkImageMemoryBarrier& barrier)
	{
		vkCmdPipelineBarrier(m_command_buffer,
			source_stage, destination_stage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);
	}
}