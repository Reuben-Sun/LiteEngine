#include "Render.h"

namespace ToolEngine
{
	Renderer::Renderer(RHIContext& rhi_context) : m_rhi_context(rhi_context)
	{
		LOG_INFO("Create Renderer!")
		VkFormat color_format = m_rhi_context.m_swapchain->getFormat();
		VkFormat depth_format = m_rhi_context.m_device->getDepthFormatDetail();
		/*m_render_pass = std::make_unique<ForwardPass>(*m_rhi_context.m_device, color_format, depth_format);
		RHIRenderTarget render_target;
		render_target.m_color_format = color_format;
		render_target.m_depth_format = depth_format;
		render_target.m_extent = VkExtent2D(m_rhi_context.m_swapchain->getWidth(), m_rhi_context.m_swapchain->getHeight());
		render_target.m_render_pass = m_render_pass.get();
		m_blit_pipeline = std::make_unique<BlitPipeline>(*m_rhi_context.m_device, render_target);*/
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::tick()
	{
		//m_blit_pipeline->renderTick();
	}
}