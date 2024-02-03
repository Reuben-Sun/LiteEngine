#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIRenderPass.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace ToolEngine
{
	struct UIContext
	{
		std::vector<float> camera_quat;
		std::vector<float> camera_pos;
	};
	class RenderUI
	{
	public:
		RenderUI(RHIContext& rhi_context, RHIRenderPass& render_pass, RHIDescriptorSet& descriptor_set);
		~RenderUI();

		void tick(RHICommandBuffer& cmd, uint32_t frame_index);

		uint32_t m_scene_width = 1920;
		uint32_t m_scene_height = 1080;
		bool need_resize = true;
		UIContext m_ui_context;
	private:
		RHIContext& m_rhi_context;
		RHIDescriptorSet& m_descriptor_set;

		void drawMainMenuBar();
		void drawHierarchy();
		void drawScene();
		void drawBrowser();
		void drawDetail();

		void setStyle();
	};
}