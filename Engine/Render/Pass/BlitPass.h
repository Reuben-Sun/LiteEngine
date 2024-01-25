#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIRenderPass.h"

namespace ToolEngine
{
	class BlitPass : public RHIRenderPass
	{
	public:
		BlitPass(RHIDevice& device, VkFormat color_format);
		~BlitPass();
	};
}