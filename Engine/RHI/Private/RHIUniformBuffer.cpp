#include "RHI/Public/RHIUniformBuffer.h"

namespace ToolEngine
{
	RHIUniformBuffer::RHIUniformBuffer(RHIDevice& device, size_t buffer_size): m_device(device), m_buffer_size(buffer_size)
	{
		m_device.createBuffer(m_buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_buffer, m_memory);
		vkMapMemory(m_device.getLogicalDevice(), m_memory, 0, m_buffer_size, 0, &m_buffer_mapped);
		
		m_descriptor.buffer = m_buffer;
		m_descriptor.offset = 0;
		m_descriptor.range = m_buffer_size;
	}
	RHIUniformBuffer::~RHIUniformBuffer()
	{
		if (m_buffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(m_device.getLogicalDevice(), m_buffer, nullptr);
		}
		if (m_memory != VK_NULL_HANDLE)
		{
			vkFreeMemory(m_device.getLogicalDevice(), m_memory, nullptr);
		}
	}
	void RHIUniformBuffer::updateBuffer(void* ubo)
	{
		memcpy(m_buffer_mapped, ubo, m_buffer_size);
	}
}