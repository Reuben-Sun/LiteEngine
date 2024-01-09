#include "RHI/Public/RHIPipeline.h"

namespace ToolEngine
{
	RHIPipeline::RHIPipeline(RHIDevice& device): m_device(device)
	{
	}
	RHIPipeline::~RHIPipeline()
	{
		if (m_pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(m_device.getLogicalDevice(), m_pipeline, nullptr);
		}
	}
}