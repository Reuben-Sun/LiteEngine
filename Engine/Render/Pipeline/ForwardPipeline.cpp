#include "ForwardPipeline.h"
#include "RHI/Public/RHIShader.h"

namespace ToolEngine
{
	ForwardPipeline::ForwardPipeline(RHIDevice& device, RHISwapchain& swapchain, uint32_t frames_count)
		: m_device(device), m_swapchain(swapchain), m_frames_count(frames_count)
	{
		createPipeline();
		LOG_INFO("Create ForwardPipeline!");
	}
	ForwardPipeline::~ForwardPipeline()
	{
	}
	void ForwardPipeline::createPipeline()
	{
		RHIShader vertex_shader_module(m_device, "Unlit_vert.spv");
		RHIShader fragment_shader_module(m_device, "Unlit_frag.spv");
	}
}