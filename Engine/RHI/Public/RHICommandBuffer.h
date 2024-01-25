#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"
#include "RHI/Public/RHIRenderPass.h"
#include "RHI/Public/RHIFrameBuffer.h"
#include "RHI/Public/RHISwapchain.h"
#include "RHI/Public/RHIVertexBuffer.h"
#include "RHI/Public/RHIIndexBuffer.h"

namespace ToolEngine
{
	class RHICommandBuffer
	{
	public:
		RHICommandBuffer(RHIDevice& device, uint32_t command_buffer_count);
		~RHICommandBuffer();

		VkCommandBuffer getHandle(uint32_t index) const { return m_command_buffers[index]; }
		uint32_t getCommandBufferCount() const { return m_command_buffer_count; }

		void resetCommandBuffer(uint32_t current_frame);
		void submitCommandBuffer(uint32_t current_frame, std::vector<VkSemaphore>& wait_semaphores, std::vector<VkSemaphore>& signal_semaphores, VkFence in_flight_fence);
		void beginRecord(uint32_t current_frame);
		void endRecord(uint32_t current_frame);
		void beginRenderPass(uint32_t current_frame, RHIRenderPass& render_pass, RHIFrameBuffer& frame_buffer, uint32_t width, uint32_t height);
		void endRenderPass(uint32_t current_frame);
		void bindPipeline(uint32_t current_frame, VkPipeline pipeline);
		void setViewport(uint32_t current_frame, float w_start, float width, float h_start, float height, float min_depth, float max_depth, uint32_t first_viewport_index, uint32_t viewport_count);
		void setScissor(uint32_t current_frame, uint32_t w_start, uint32_t width, uint32_t h_start, uint32_t height, uint32_t first_scissor_index, uint32_t scissor_count);
		void draw(uint32_t current_frame, uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex_index, uint32_t first_instance_index);
		void drawIndexed(uint32_t current_frame, uint32_t index_count, uint32_t instance_count, uint32_t first_index, uint32_t vertex_offset, uint32_t first_instance);
		void bindVertexBuffer(uint32_t current_frame, RHIVertexBuffer& buffer, VkDeviceSize* offset, uint32_t first_binding_index, uint32_t binding_count);
		void bindIndexBuffer(uint32_t current_frame, RHIIndexBuffer& buffer, VkDeviceSize offset, VkIndexType index_type);
		void bindDescriptorSets(uint32_t current_frame, VkPipelineBindPoint bind_point, VkPipelineLayout layout, const std::vector<VkDescriptorSet> descriptor_sets, uint32_t first_set_index, uint32_t descriptor_set_count);
	private:
		RHIDevice& m_device;
		std::vector<VkCommandBuffer> m_command_buffers;
		uint32_t m_command_buffer_count;
	};
}