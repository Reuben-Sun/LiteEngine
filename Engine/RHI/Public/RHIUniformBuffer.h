#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHIUniformBuffer
	{
	public:
		RHIUniformBuffer(RHIDevice& device, size_t buffer_size);
		~RHIUniformBuffer();

		void updateBuffer(void* ubo);
		VkBuffer getHandle() const { return m_buffer; }
		VkDescriptorBufferInfo m_descriptor;
	private:
		RHIDevice& m_device;

		size_t m_buffer_size;
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		void* m_buffer_mapped;
	};
}