#pragma once

#include "Marco.h"
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include "RHI/Public/RHIContext.h"
#include "Render/Pass/UIPass.h"
#include "RHI/Public/RHIFrameBuffer.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "RHI/Public/RHITextureImage.h"
#include "Render/Resources/RenderScene.h"

namespace ToolEngine
{
	struct UIContext
	{
		uint32_t m_scene_width = 1920;
		uint32_t m_scene_height = 1080;
		bool need_resize = true;
		bool enable_gizmos = false;
		int debug_mode = 0;
	};

	class EditorUI
	{
	public:
		EditorUI(RHIContext& rhi_context, UIContext& ui_context, RenderScene& scene);
		~EditorUI();

		void record(RHICommandBuffer& cmd, uint32_t frame_index, RHIDescriptorSet& scene_image);
		bool m_full_screen = false;
		glm::vec4 m_scene_bounding;	// begin x, begin y, end x, end y
	private:
		RHIContext& m_rhi_context;
		UIContext& m_ui_context;
		RenderScene& m_scene;

		std::unique_ptr<UIPass> m_ui_pass;
		std::vector<std::unique_ptr<RHIFrameBuffer>> m_ui_frame_buffers;
		std::unique_ptr<RHIDescriptorSetLayout> m_texture_descriptor_set_layout;
		std::unordered_map<std::string, std::unique_ptr<RHITextureImage>> m_texture_name_to_image;
		std::unordered_map<std::string, std::unique_ptr<RHIDescriptorSet>> m_texture_name_to_descriptor_set;
		std::string m_current_path;
		float m_browser_button_size = 100.0f;
		float m_browser_button_spacing = 10.0f;

		void initImGui();
		void setImGuiStyle();
		void drawMainMenuBar();
		void drawHierarchy();
		void drawScene(RHIDescriptorSet& scene_image);
		void drawBrowser();
		void drawDetail();
		std::string selectIcon(const std::string& file_name);
	};
}