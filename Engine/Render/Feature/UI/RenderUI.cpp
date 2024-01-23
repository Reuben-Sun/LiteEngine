#include "RenderUI.h"
#include "RHI/Public/RHISingleTimeCommandBuffer.h"

namespace ToolEngine
{
	RenderUI::RenderUI(RHIContext& rhi_context, RHIRenderPass& render_pass): m_rhi_context(rhi_context)
	{
		ImGui::CreateContext();
		//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
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
	void RenderUI::drawScene(uint32_t width, uint32_t height)
	{
		auto scene_extent = getSceneExtent(width, height);
		ImGui::SetNextWindowPos(ImVec2(scene_extent[0], scene_extent[2]));
		ImGui::SetNextWindowSize(ImVec2(scene_extent[1], scene_extent[3]));
		ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		ImGui::End();
	}
	void RenderUI::drawBrowser(uint32_t width, uint32_t height)
	{
		auto browser_extent = getBrowserExtent(width, height);
		ImGui::SetNextWindowPos(ImVec2(browser_extent[0], browser_extent[2]));
		ImGui::SetNextWindowSize(ImVec2(browser_extent[1], browser_extent[3]));
		ImGui::Begin("Browser", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		ImGui::End();
	}
	void RenderUI::drawDetail(uint32_t width, uint32_t height)
	{
		auto detail_extent = getDetailExtent(width, height);
		ImGui::SetNextWindowPos(ImVec2(detail_extent[0], detail_extent[2]));
		ImGui::SetNextWindowSize(ImVec2(detail_extent[1], detail_extent[3]));
		ImGui::Begin("Detail", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		ImGui::End();
	}
	void RenderUI::tick(RHICommandBuffer& cmd, uint32_t frame_index)
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		uint32_t width = m_rhi_context.m_swapchain->getWidth();
		uint32_t height = m_rhi_context.m_swapchain->getHeight();

		drawScene(width, height);
		drawBrowser(width, height);
		drawDetail(width, height);
		
		ImGui::Render();

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd.getHandle(frame_index));
		ImGuiIO& io = ImGui::GetIO();
	}
}