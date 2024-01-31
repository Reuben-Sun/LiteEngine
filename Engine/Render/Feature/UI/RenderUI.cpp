#include "RenderUI.h"
#include "RHI/Public/RHISingleTimeCommandBuffer.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
	RenderUI::RenderUI(RHIContext& rhi_context, RHIRenderPass& render_pass): m_rhi_context(rhi_context)
	{
		ImGui::CreateContext();
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui_ImplGlfw_InitForVulkan(rhi_context.m_window.getHandle(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = rhi_context.m_instance->getHandle();
		init_info.PhysicalDevice = rhi_context.m_device->getPhysicalDevice();
		init_info.Device = rhi_context.m_device->getLogicalDevice();
		init_info.QueueFamily = rhi_context.m_device->getGraphicsFamilyIndex();
		init_info.Queue = rhi_context.m_device->getGraphicsQueue();
		init_info.DescriptorPool = rhi_context.m_descriptor_pool->getHandle();
		init_info.MinImageCount = rhi_context.m_swapchain->getImageCount();	// 3
		init_info.ImageCount = rhi_context.m_swapchain->getImageCount();	// 3;
		ImGui_ImplVulkan_Init(&init_info, render_pass.getHandle());
	}
	RenderUI::~RenderUI()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	std::vector<float> RenderUI::getDisplayExtent(uint32_t width, uint32_t height)
	{
		std::vector<float> extent(4);
		extent[0] = width * m_left_padding;
		extent[1] = width * (1 - m_left_padding - m_right_padding);
		extent[2] = height * m_top_padding;
		extent[3] = height * (1 - m_top_padding - m_bottom_padding);
		return extent;
	}
	std::vector<float> RenderUI::getSceneExtent(uint32_t width, uint32_t height)
	{
		std::vector<float> extent(4);
		extent[0] = 0;
		extent[1] = width * m_left_padding;
		extent[2] = 0;
		extent[3] = height * (1 - m_bottom_padding);
		return extent;
	}
	std::vector<float> RenderUI::getBrowserExtent(uint32_t width, uint32_t height)
	{
		std::vector<float> extent(4);
		extent[0] = 0;
		extent[1] = width * (1 - m_right_padding);
		extent[2] = height * (1 - m_bottom_padding);
		extent[3] = height * m_bottom_padding;
		return extent;
	}
	std::vector<float> RenderUI::getDetailExtent(uint32_t width, uint32_t height)
	{
		std::vector<float> extent(4);
		extent[0] = width * (1 - m_right_padding);
		extent[1] = width * m_right_padding;
		extent[2] = 0;
		extent[3] = height;
		return extent;
	}
	void RenderUI::drawHierarchy(uint32_t width, uint32_t height)
	{
		ImGui::Begin("Hierarchy");
		ImGui::End();
	}
	void RenderUI::drawScene(uint32_t width, uint32_t height)
	{
		//ImGui::Begin("SceneView");
		///*auto color_descriptor_set = ImGui_ImplVulkan_AddTexture(color_sampler, color_image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		//ImGui::Image((ImTextureID)color_descriptor_set, ImVec2(width, height));*/
		//ImGui::End();
	}
	void RenderUI::drawBrowser(uint32_t width, uint32_t height)
	{
		ImGui::Begin("Browser");
		ImGui::End();
	}
	void RenderUI::drawDetail(uint32_t width, uint32_t height)
	{
		ImGui::Begin("Detail");
		ImGui::End();
	}
	void RenderUI::tick(RHICommandBuffer& cmd, uint32_t frame_index, RHIDescriptorSet& descriptor_set)
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		uint32_t width = m_rhi_context.m_swapchain->getWidth();
		uint32_t height = m_rhi_context.m_swapchain->getHeight();

		drawHierarchy(width, height);
		drawScene(width, height);
		drawBrowser(width, height);
		drawDetail(width, height);
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Create")) {
				}
				if (ImGui::MenuItem("Open", "Ctrl+O")) {
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
				}
				if (ImGui::MenuItem("Save as..")) {
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		ImGui::Begin("SceneView");
		ImVec2 window_size = ImGui::GetWindowSize();
		if (m_scene_width != (uint32_t)window_size.x || m_scene_height != (uint32_t)window_size.y)
		{
			m_scene_width = window_size.x;
			m_scene_height = window_size.y;
			need_resize = true;
			LOG_INFO("scene width: {0}, scene height: {1}", m_scene_width, m_scene_height);
		}
		
		
		ImGui::Image(descriptor_set.getHandle(), window_size);
		ImGui::End();
		ImGui::Render();

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd.getHandle(frame_index));
	}
}