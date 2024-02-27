#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHIComputePipeline
	{
	public:
		RHIComputePipeline(RHIDevice& device);
		~RHIComputePipeline();
	private:
		RHIDevice& m_device;
	};
}