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

		m_forward_pipeline = std::make_unique<ForwardPipeline>(*m_rhi_context.m_device, *m_rhi_context.m_swapchain, MAX_FRAMES_IN_FLIGHT);
		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_frame_buffers.emplace_back(std::make_unique<RHIFrameBuffer>(*m_rhi_context.m_device, 
				m_forward_pipeline->getRenderPass().getHandle(), 
				m_rhi_context.m_swapchain->getImageView(i), 
				m_depth_resources->getImageView(), width, height));
		}

		m_command_buffer = std::make_unique<RHICommandBuffer>(*m_rhi_context.m_device, MAX_FRAMES_IN_FLIGHT);
		for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_in_flight_fences.emplace_back(std::make_unique<Fence>(*m_rhi_context.m_device));
			m_image_available_semaphores.emplace_back(std::make_unique<Semaphore>(*m_rhi_context.m_device));
			m_render_finished_semaphores.emplace_back(std::make_unique<Semaphore>(*m_rhi_context.m_device));
		}
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::tick()
	{
		uint32_t frame_index = getFrameIndex();
		//m_in_flight_fences[frame_index]->wait();

		//m_blit_pipeline->renderTick();
	}
}