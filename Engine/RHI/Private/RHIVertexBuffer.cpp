#include "RHI/Public/RHIVertexBuffer.h"

namespace ToolEngine
{
	RHIVertexBuffer::RHIVertexBuffer(RHIDevice& device, std::vector<Vertex>& vertex_buffer): m_device(device)
	{
		VkDeviceSize buffer_size = sizeof(vertex_buffer[0]) * vertex_buffer.size();
		// create staging buffer in CPU memory
		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_memory;
		m_device.createBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
			staging_buffer, staging_buffer_memory);
		vkMapMemory(m_device.getLogicalDevice(), staging_buffer_memory, 0, buffer_size, 0, &m_buffer_mapped);
		memcpy(m_buffer_mapped, vertex_buffer.data(), (size_t)buffer_size);
		vkUnmapMemory(m_device.getLogicalDevice(), staging_buffer_memory);
		// create destination buffer in device memory
		m_device.createBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_memory);
		// use vulkan command to copy buffer from staging buffer to destination buffer
		m_device.copyBuffer(staging_buffer, m_buffer, buffer_size);
		// destroy staging buffer
		vkDestroyBuffer(m_device.getLogicalDevice(), staging_buffer, nullptr);
		vkFreeMemory(m_device.getLogicalDevice(), staging_buffer_memory, nullptr);
	}
	RHIVertexBuffer::~RHIVertexBuffer()
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
	void RHIVertexBuffer::updateBuffer(std::vector<Vertex>& vertex_buffer)
	{
		VkDeviceSize buffer_size = sizeof(vertex_buffer[0]) * vertex_buffer.size();
		memcpy(m_buffer_mapped, vertex_buffer.data(), (size_t)buffer_size);
	}
}