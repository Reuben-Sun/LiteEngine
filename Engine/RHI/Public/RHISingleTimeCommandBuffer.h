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

		void pipelineBarrier();

	private:
		RHIDevice& m_device;
		VkCommandBuffer m_command_buffer;
	};
}