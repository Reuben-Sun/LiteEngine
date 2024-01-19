#include "Render.h"

namespace ToolEngine
{
	Renderer::Renderer(RHIContext& rhi_context) : m_rhi_context(rhi_context)
	{
		LOG_INFO("Create Renderer!")
		VkFormat color_format = m_rhi_context.m_swapchain->getFormat();
		VkFormat depth_format = m_rhi_context.m_device->getDepthFormatDetail();
		uint32_t width = m_rhi_context.m_swapchain->getWidth();
		uint32_t height = m_rhi_context.m_swapchain->getHeight();
		m_depth_resources = std::make_unique<DepthResources>(*m_rhi_context.m_device, width, height);

		m_forward_pipeline = std::make_unique<ForwardPipeline>(*m_rhi_context.m_device, *m_rhi_context.m_swapchain, *rhi_context.m_descriptor_pool, m_max_frames_in_flight);
		uint32_t swapchain_image_count = m_rhi_context.m_swapchain->getImageCount();
		m_max_frames_in_flight = swapchain_image_count;
		for (uint32_t i = 0; i < swapchain_image_count; i++)
		{
			m_frame_buffers.emplace_back(std::make_unique<RHIFrameBuffer>(*m_rhi_context.m_device, 
				m_forward_pipeline->getRenderPass().getHandle(), 
				m_rhi_context.m_swapchain->getImageView(i), 
				m_depth_resources->getImageView(), width, height));
		}

		m_command_buffer = std::make_unique<RHICommandBuffer>(*m_rhi_context.m_device, m_max_frames_in_flight);
		for (uint32_t i = 0; i < swapchain_image_count; i++)
		{
			m_in_flight_fences.emplace_back(std::make_unique<Fence>(*m_rhi_context.m_device));
			m_image_available_semaphores.emplace_back(std::make_unique<Semaphore>(*m_rhi_context.m_device));
			m_render_finished_semaphores.emplace_back(std::make_unique<Semaphore>(*m_rhi_context.m_device));
		}
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::tick(RenderScene& scene)
	{
		uint32_t frame_index = getFrameIndex();
		m_in_flight_fences[frame_index]->wait();
		uint32_t image_index;
		VkResult result = vkAcquireNextImageKHR(m_rhi_context.m_device->getLogicalDevice(), 
			m_rhi_context.m_swapchain->getHandle(), UINT64_MAX, 
			m_image_available_semaphores[frame_index]->getHandle(), VK_NULL_HANDLE, &image_index);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			//resizeFrame();
			return;
		}
		else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			LOG_ERROR("failed to acquire swap chain image!");
		}

		m_in_flight_fences[frame_index]->resetFence();

		m_command_buffer->resetCommandBuffer(frame_index);

		m_forward_pipeline->tick(*m_command_buffer, *m_frame_buffers[frame_index], frame_index, scene);

		std::vector<VkSemaphore> wait_semaphores { m_image_available_semaphores[frame_index]->getHandle() };
		std::vector<VkSemaphore> signal_semaphores { m_render_finished_semaphores[frame_index]->getHandle() };
		m_command_buffer->submitCommandBuffer(frame_index, wait_semaphores, 
			signal_semaphores, m_in_flight_fences[frame_index]->getHandle());

		std::vector<VkSwapchainKHR> swapchains{ m_rhi_context.m_swapchain->getHandle() };
		m_rhi_context.m_device->present(signal_semaphores, image_index, swapchains);
		m_current_frame++;
	}
}