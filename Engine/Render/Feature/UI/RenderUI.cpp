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
	void RenderUI::tick(RHICommandBuffer& cmd, uint32_t frame_index)
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		uint32_t width = m_rhi_context.m_swapchain->getWidth();
		uint32_t height = m_rhi_context.m_swapchain->getHeight();

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(width * m_left_padding, height * (1 - m_bottom_padding)));
		ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(0, height * (1 - m_bottom_padding)));
		ImGui::SetNextWindowSize(ImVec2(width * (1 - m_right_padding), height * m_bottom_padding));
		ImGui::Begin("File", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		ImGui::End();

		ImGui::SetNextWindowPos(ImVec2(width * (1 - m_right_padding), 0));
		ImGui::SetNextWindowSize(ImVec2(width * m_right_padding, height));
		ImGui::Begin("Detail", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		ImGui::End();

		ImGui::Render();


		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd.getHandle(frame_index));
		ImGuiIO& io = ImGui::GetIO();
		/*if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}*/
	}
}