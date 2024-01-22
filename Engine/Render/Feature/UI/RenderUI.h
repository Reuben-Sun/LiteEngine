#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIRenderPass.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace ToolEngine
{
	class RenderUI
	{
	public:
		RenderUI(RHIContext& rhi_context, RHIRenderPass& render_pass);
		~RenderUI();

		void tick(RHICommandBuffer& cmd, uint32_t frame_index);
	private:

	};
}