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
		glm::vec3 camera_position;
	};

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