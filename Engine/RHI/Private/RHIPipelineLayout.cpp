#include "RHI/Public/RHIPipelineLayout.h"

namespace ToolEngine
{
	RHIPipelineLayout::RHIPipelineLayout(RHIDevice& device, const VkPipelineLayoutCreateInfo& info)
		: m_device(device)
	{
		if (vkCreatePipelineLayout(m_device.getLogicalDevice(), &info, nullptr, &m_pipeline_layout) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create pipeline layout!");
		}
		LOG_INFO("Create Pipeline Layout!");
	}
	RHIPipelineLayout::~RHIPipelineLayout()
	{
		if (m_pipeline_layout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(m_device.getLogicalDevice(), m_pipeline_layout, nullptr);
		}
	}
}