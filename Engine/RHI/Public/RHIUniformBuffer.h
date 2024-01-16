#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "RHIDevice.h"
#include "Geometry/Vertex.h"

namespace ToolEngine
{
	struct GlobalUBO
	{
		glm::mat4 model_matrix;		
		glm::mat4 view_matrix;
		glm::mat4 projection_matrix;
	};

	class RHIUniformBuffer
	{
	public:
		RHIUniformBuffer(RHIDevice& device);
		~RHIUniformBuffer();

		void updateBuffer(GlobalUBO& ubo);
		VkBuffer getHandle() const { return m_buffer; }
		VkDescriptorBufferInfo m_descriptor;
	private:
		RHIDevice& m_device;
		VkBuffer m_buffer;
		VkDeviceMemory m_memory;
		void* m_buffer_mapped;
	};
}