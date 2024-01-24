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

		// 0 is w_start, 1 is w_width, 2 is h_start, 3 is h_height
		std::vector<float> getDisplayExtent(uint32_t width, uint32_t height);
		std::vector<float> getSceneExtent(uint32_t width, uint32_t height);
		std::vector<float> getBrowserExtent(uint32_t width, uint32_t height);
		std::vector<float> getDetailExtent(uint32_t width, uint32_t height);

		void tick(RHICommandBuffer& cmd, uint32_t frame_index, VkImageView color_image, VkSampler color_sampler);
	private:
		RHIContext& m_rhi_context;
		float m_left_padding = 0.2;
		float m_right_padding = 0.2;
		float m_top_padding = 0;
		float m_bottom_padding = 0.4;

		void drawHierarchy(uint32_t width, uint32_t height);
		void drawScene(uint32_t width, uint32_t height);
		void drawBrowser(uint32_t width, uint32_t height);
		void drawDetail(uint32_t width, uint32_t height);
	};
}