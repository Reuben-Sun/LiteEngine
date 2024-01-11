#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"
#include "Geometry/Vertex.h"

namespace ToolEngine
{
	class RHIVertexBuffer
	{
	public:
		RHIVertexBuffer(RHIDevice& device, std::vector<Vertex>& vertex_buffer);
		~RHIVertexBuffer();

		void updateBuffer(std::vector<Vertex>& vertex_buffer);
		VkBuffer getHandle() const { return m_buffer; }
	private:
		RHIDevice& m_device;
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		void* m_buffer_mapped;
	};
}