#pragma once

#include "Marco.h"
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>
#include "RHI/Public/RHIContext.h"
#include "Render/Editor/EditorUI.h"
#include "Render/Runtime/Render.h"
#include "Render/Pass/UIPass.h"
#include "RHI/Public/RHIFrameBuffer.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "RHI/Public/RHITextureImage.h"
#include "Geometry/RenderScene.h"

namespace ToolEngine
{
	class RenderContext
	{
	public:
		RenderContext(RHIContext& rhi_context);
		~RenderContext();

		void tick(RenderScene& scene);
	private:
		RHIContext& m_rhi_context;
		
		std::unique_ptr<RHICommandBuffer> m_command_buffer;
		std::unique_ptr<UIPass> m_ui_pass;
		std::vector<std::unique_ptr<RHIFrameBuffer>> m_ui_frame_buffers;
		std::vector<std::unique_ptr<Fence>> m_in_flight_fences;
		std::vector<std::unique_ptr<Semaphore>> m_image_available_semaphores;
		std::vector<std::unique_ptr<Semaphore>> m_render_finished_semaphores;

		uint32_t m_max_frames_in_flight{ 3 };
		uint32_t m_current_frame{ 0 };
		uint32_t getFrameIndex() const { return m_current_frame % m_max_frames_in_flight; }

		bool prepareFrame(uint32_t& image_index);
		void submitFrame(uint32_t image_index);

		std::unique_ptr<RHIDescriptorSetLayout> m_texture_descriptor_set_layout;
		std::unordered_map<std::string, std::unique_ptr<RHITextureImage>> m_texture_name_to_image;
		std::unordered_map<std::string, std::unique_ptr<RHIDescriptorSet>> m_texture_name_to_descriptor_set;
		std::string m_current_path;
		float m_browser_button_size = 100.0f;
		float m_browser_button_spacing = 10.0f;
		void initImGui();
		void setImGuiStyle();
		void drawUI();
		void drawMainMenuBar();
		void drawHierarchy();
		void drawScene();
		void drawBrowser();
		void drawDetail();
		std::string selectIcon(const std::string& file_name);
	};
}