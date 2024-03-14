#include "RenderPipeline.h"

namespace ToolEngine
{
	RenderPipeline::RenderPipeline(RHIDevice& device, VkRenderPass render_pass)
		: m_device(device), m_render_pass(render_pass)
	{
	}
	RenderPipeline::~RenderPipeline()
	{
	}
}