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

	private:
		RHIDevice& m_device;
		VkCommandBuffer m_command_buffer;
	};
}