#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"
#include "RHIPipelineLayout.h"
#include "RHIDescriptorSet.h"

namespace ToolEngine
{
	class RHIComputePipeline
	{
	public:
		RHIComputePipeline(RHIDevice& device);
		~RHIComputePipeline();
	private:
		RHIDevice& m_device;
		std::unique_ptr<RHIDescriptorSetLayout> m_descriptor_set_layout;
		std::unique_ptr<RHIPipelineLayout> m_pipeline_layout;

		void createPipeline();
	};
}