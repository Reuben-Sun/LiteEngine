#include "ForwardPipeline.h"

namespace ToolEngine
{
	ForwardPipeline::ForwardPipeline(RHIDevice& device, RHISwapchain& swapchain, uint32_t frames_count)
		: m_device(device), m_swapchain(swapchain), m_frames_count(frames_count)
	{
		// TODO: createPipeline()
	}
	ForwardPipeline::~ForwardPipeline()
	{
	}
}