#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHIIndexBuffer
	{
	public:
		RHIIndexBuffer(RHIDevice& device, std::vector<uint32_t>& index_buffer);
		~RHIIndexBuffer();

		void updateBuffer(std::vector<uint32_t>& index_buffer);
		VkBuffer getHandle() const { return m_buffer; }
	private:
		RHIDevice& m_device;
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		void* m_buffer_mapped;
	};
}