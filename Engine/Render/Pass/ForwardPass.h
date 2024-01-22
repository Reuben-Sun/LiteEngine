#pragma once

#include "Marco.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIRenderPass.h"

namespace ToolEngine
{
	class ForwardPass : public RHIRenderPass
	{
	public:
		ForwardPass(RHIDevice& device, VkFormat color_format, VkFormat depth_format);
		~ForwardPass();
	};
}
