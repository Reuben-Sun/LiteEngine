#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHISingleTimeCommandBuffer
	{
	public:
		RHISingleTimeCommandBuffer(RHIDevice& device);
		~RHISingleTimeCommandBuffer();

		void pipelineBarrier(VkPipelineStageFlags source_stage, VkPipelineStageFlags destination_stage, VkImageMemoryBarrier& barrier);
		void copyBuffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);
		void copyImage(VkBuffer src_buffer, VkImage dst_image, VkImageLayout image_layout, uint32_t region_count, const VkBufferImageCopy* regions);
	private:
		RHIDevice& m_device;
		VkCommandBuffer m_command_buffer;
	};
}