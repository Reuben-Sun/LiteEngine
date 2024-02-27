#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIRenderPass.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "RHI/Public/RHITextureImage.h"
#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace ToolEngine
{
	// TODO: multi-thread swap context
	struct UIContext
	{
		std::vector<float> camera_rotation;
		std::vector<float> camera_pos;
		float camera_speed;
		std::vector<float> cube_pos;
		std::vector<float> cube_rotation;
		uint32_t m_scene_width = 1920;
		uint32_t m_scene_height = 1080;
		bool need_resize = true;
		bool enable_gizmos = true;
		float metallic = 0.134f;
		float roughness = 0.276f;
	};
	class RenderUI
	{
	public:
		RenderUI(RHIContext& rhi_context, RHIRenderPass& render_pass, RHIDescriptorSet& descriptor_set);
		~RenderUI();

		void tick(RHICommandBuffer& cmd, uint32_t frame_index);

		UIContext& getUIContext() { return m_ui_context; }
	private:
		RHIContext& m_rhi_context;
		RHIDescriptorSet& m_descriptor_set;

		std::string m_current_path;
		float m_browser_button_size = 100.0f;
		float m_browser_button_spacing = 10.0f;

		std::unique_ptr<RHIDescriptorSetLayout> m_texture_descriptor_set_layout;
		std::unordered_map<std::string, std::unique_ptr<RHITextureImage>> m_texture_name_to_image;
		std::unordered_map<std::string, std::unique_ptr<RHIDescriptorSet>> m_texture_name_to_ubo_descriptor_set;
		
		UIContext m_ui_context;
		
		void drawMainMenuBar();
		void drawHierarchy();
		void drawScene();
		void drawBrowser();
		void drawDetail();

		void setStyle();

		std::string selectIcon(const std::string& file_name);
	};
}