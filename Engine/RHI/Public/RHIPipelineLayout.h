#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHIPipelineLayout
	{
	public:
		RHIPipelineLayout(RHIDevice* device, const VkPipelineLayoutCreateInfo& info);
		~RHIPipelineLayout();

		VkPipelineLayout getHandle() const { return m_pipeline_layout; }

	private:
		VkPipelineLayout m_pipeline_layout;
		RHIDevice& m_device;
	};
}