#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"
#include "RHIPipelineState.h"

namespace ToolEngine
{
	class RHIPipeline
	{
	public:
		RHIPipeline(RHIDevice& device);
		virtual ~RHIPipeline();

		VkPipeline getHandle() const { return m_pipeline; }
		void createPipeline(RHIPipelineState& pipeline_state);
		const RHIPipelineState& getPipelineState() const { return m_pipeline_state; }

	protected:
		VkPipeline m_pipeline{ VK_NULL_HANDLE };
		RHIDevice& m_device;
		RHIPipelineState m_pipeline_state;
	};
}