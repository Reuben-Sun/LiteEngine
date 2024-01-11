#include "RHI/Public/RHICommandBuffer.h"

namespace ToolEngine
{
	RHICommandBuffer::RHICommandBuffer(RHIDevice& device, uint32_t command_buffer_count)
		: m_device(device), m_command_buffer_count(command_buffer_count)
	{
		VkCommandBufferAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.commandPool = m_device.getCommandPool();
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandBufferCount = m_command_buffer_count;

		m_command_buffers.resize(m_command_buffer_count);
		if (vkAllocateCommandBuffers(m_device.getLogicalDevice(), &alloc_info, m_command_buffers.data()) != VK_SUCCESS)
		{
			LOG_ERROR("failed to allocate command buffers!");
		}
		LOG_INFO("Create Command Buffer!");
	}
	RHICommandBuffer::~RHICommandBuffer()
	{
	}
	void RHICommandBuffer::resetCommandBuffer(uint32_t current_frame)
	{
		vkResetCommandBuffer(m_command_buffers[current_frame], 0);
	}

	void RHICommandBuffer::submitCommandBuffer(uint32_t current_frame, std::vector<VkSemaphore>& wait_semaphores, std::vector<VkSemaphore>& signal_semaphores, VkFence in_flight_fence)
	{
		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = wait_semaphores.data();
		submit_info.pWaitDstStageMask = waitStages;

		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &m_command_buffers[current_frame];

		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = signal_semaphores.data();

		if (vkQueueSubmit(m_device.getGraphicsQueue(), 1, &submit_info, in_flight_fence) != VK_SUCCESS)
		{
			LOG_ERROR("failed to submit draw command buffer!");
		}
	}
	void RHICommandBuffer::beginRecord(uint32_t current_frame)
	{
		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(m_command_buffers[current_frame], &begin_info) != VK_SUCCESS)
		{
			LOG_ERROR("failed to begin recording command buffer!");
		}
	}
	void RHICommandBuffer::endRecord(uint32_t current_frame)
	{
		if (vkEndCommandBuffer(m_command_buffers[current_frame]) != VK_SUCCESS)
		{
			LOG_ERROR("failed to record command buffer!");
		}
	}
	void RHICommandBuffer::beginRenderPass(uint32_t current_frame, RHIRenderPass& render_pass, RHIFrameBuffer& frame_buffer, uint32_t width, uint32_t height)
	{
		VkExtent2D extent{ width, height };
		VkRenderPassBeginInfo render_pass_info{};
		render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_info.renderPass = render_pass.getHandle();
		render_pass_info.framebuffer = frame_buffer.getHandle();
		render_pass_info.renderArea.offset = { 0, 0 };
		render_pass_info.renderArea.extent = extent;
		std::vector<VkClearValue> clear_colors(2);
		clear_colors[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
		clear_colors[1].depthStencil = { 1.0f, 0 };
		render_pass_info.clearValueCount = static_cast<uint32_t>(clear_colors.size());
		render_pass_info.pClearValues = clear_colors.data();
		vkCmdBeginRenderPass(m_command_buffers[current_frame], &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
	}
	void RHICommandBuffer::endRenderPass(uint32_t current_frame)
	{
		vkCmdEndRenderPass(m_command_buffers[current_frame]);
	}
	void RHICommandBuffer::bindPipeline(uint32_t current_frame, VkPipeline pipeline)
	{
		vkCmdBindPipeline(m_command_buffers[current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	}
	void RHICommandBuffer::setViewport(uint32_t current_frame, uint32_t width, uint32_t height, float min_depth, float max_depth, uint32_t first_viewport_index, uint32_t viewport_count)
	{
		VkExtent2D extent{ width, height };
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = extent.width;
		viewport.height = extent.height;
		viewport.minDepth = min_depth;
		viewport.maxDepth = max_depth;
		vkCmdSetViewport(m_command_buffers[current_frame], first_viewport_index, viewport_count, &viewport);
	}
	void RHICommandBuffer::setScissor(uint32_t current_frame, uint32_t width, uint32_t height, uint32_t first_scissor_index, uint32_t scissor_count)
	{
		VkExtent2D extent{ width, height };
		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = extent;
		vkCmdSetScissor(m_command_buffers[current_frame], first_scissor_index, scissor_count, &scissor);
	}
	void RHICommandBuffer::draw(uint32_t current_frame, uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex_index, uint32_t first_instance_index)
	{
		vkCmdDraw(m_command_buffers[current_frame], vertex_count, instance_count, first_vertex_index, first_instance_index);
	}
	void RHICommandBuffer::draw(uint32_t current_frame, uint32_t index_count, uint32_t instance_count, uint32_t first_index, uint32_t vertex_offset, uint32_t first_instance)
	{
		vkCmdDrawIndexed(m_command_buffers[current_frame], index_count, instance_count, first_index, vertex_offset, first_instance);
	}
	void RHICommandBuffer::bindVertexBuffer(uint32_t current_frame, RHIVertexBuffer& buffer, VkDeviceSize* offset, uint32_t first_binding_index, uint32_t binding_count)
	{
		VkBuffer vertex_buffers[] = { buffer.getHandle() };
		vkCmdBindVertexBuffers(m_command_buffers[current_frame], first_binding_index, binding_count, vertex_buffers, offset);
	}
	void RHICommandBuffer::bindIndexBuffer(uint32_t current_frame, RHIIndexBuffer& buffer, VkDeviceSize offset, VkIndexType index_type)
	{
		vkCmdBindIndexBuffer(m_command_buffers[current_frame], buffer.getHandle(), offset, index_type);
	}
}