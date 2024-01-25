#include "BlitPipeline.h"

namespace ToolEngine
{
	BlitPipeline::BlitPipeline(RHIDevice& device, VkRenderPass render_pass): m_device(device), m_render_pass(render_pass)
	{
	}
	BlitPipeline::~BlitPipeline()
	{
	}
	void BlitPipeline::createPipeline()
	{
	}
}