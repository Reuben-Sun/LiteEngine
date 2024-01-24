#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIRenderPass.h"

namespace ToolEngine
{
	class UIPass : public RHIRenderPass
	{
	public:
		UIPass(RHIDevice& device, VkFormat color_format);
		~UIPass();
	};
}