#include "RenderUI.h"
#include "RHI/Public/RHISingleTimeCommandBuffer.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
	RenderUI::RenderUI(RHIContext& rhi_context, RHIRenderPass& render_pass, RHIDescriptorSet& descriptor_set)
		: m_rhi_context(rhi_context), m_descriptor_set(descriptor_set)
	{
		// imgui init
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		std::string ttf_path = Path::getInstance().getAssetPath() + "MiSans-Medium.ttf";
		io.Fonts->AddFontFromFileTTF(ttf_path.c_str(), 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
		std::string ini_path = Path::getInstance().getAssetPath() + "imgui.ini";
		ImGui::LoadIniSettingsFromDisk(ini_path.c_str());
		setStyle();

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

		// editor icon init
		std::vector<RHIDescriptorType> layout_descriptor;
		layout_descriptor.push_back(RHIDescriptorType::Sampler);
		m_texture_descriptor_set_layout = std::make_unique<RHIDescriptorSetLayout>(*rhi_context.m_device, layout_descriptor);
		std::string icon_path = Path::getInstance().getAssetPath() + "Icon\\";
		auto icons = Path::getInstance().getAllFilesInDirectory(icon_path);
		for(auto& icon : icons)
		{
			auto name = Path::getInstance().getFileNameWithoutExtension(icon);
			m_texture_name_to_image[name] = std::make_unique<RHITextureImage>(*rhi_context.m_device, icon);
			m_texture_name_to_ubo_descriptor_set[name] = std::make_unique<RHIDescriptorSet>(*rhi_context.m_device, 
				*rhi_context.m_descriptor_pool, *m_texture_descriptor_set_layout);
			m_texture_name_to_ubo_descriptor_set[name]->updateTextureImage(m_texture_name_to_image[name]->m_descriptor, 0);
		}
		m_current_path = Path::getInstance().getAssetPath();
	}
	RenderUI::~RenderUI()
	{
		std::string ini_path = Path::getInstance().getAssetPath() + "imgui.ini";
		ImGui::SaveIniSettingsToDisk(ini_path.c_str());

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void RenderUI::drawMainMenuBar()
	{
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
	}
	void RenderUI::drawHierarchy()
	{
		ImGui::Begin("Hierarchy");
		// TODO: scene is manager by a json, parse it and show it in the hierarchy
		// each item is a button, click it to select the object
		ImGui::End();
	}
	void RenderUI::drawScene()
	{
		ImGui::Begin("SceneView");
		ImVec2 window_size = ImGui::GetContentRegionAvail();
		if (m_ui_context.m_scene_width != (uint32_t)window_size.x || m_ui_context.m_scene_height != (uint32_t)window_size.y)
		{
			// if window size changed, need to resize
			m_ui_context.m_scene_width = window_size.x;
			m_ui_context.m_scene_height = window_size.y;
			m_ui_context.need_resize = true;
			LOG_INFO("scene width: {0}, scene height: {1}", m_ui_context.m_scene_width, m_ui_context.m_scene_height);
		}
		// scene image
		ImGui::Image(m_descriptor_set.getHandle(), window_size);
		ImGui::End();
	}
	void RenderUI::drawBrowser()
	{
		ImGui::Begin("Browser");
		float panel_width = ImGui::GetContentRegionAvail().x;
		float cell_size = m_browser_button_size + m_browser_button_spacing;
		uint32_t column_count = (uint32_t)(panel_width / cell_size);
		if (column_count < 1)
		{
			column_count = 1;
		}
		ImGui::Columns(column_count, 0, false);
		// if not root, show back button
		if (m_current_path != Path::getInstance().getAssetPath())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::ImageButton(m_texture_name_to_ubo_descriptor_set["rollback"]->getHandle(), ImVec2(m_browser_button_size, m_browser_button_size), { 0, 0 }, { 1, 1 });
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_current_path = Path::getInstance().getDirectoryParentDirectory(m_current_path);
			}
			ImGui::NextColumn();
		}
		// show all directory
		auto dirs = Path::getInstance().getAllDirectoriesInDirectory(m_current_path);
		for (uint32_t i = 0; i < dirs.size(); i++)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::ImageButton(m_texture_name_to_ubo_descriptor_set["folder-open-fill"]->getHandle(), ImVec2(m_browser_button_size, m_browser_button_size), { 0, 0 }, { 1, 1 });
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				m_current_path = dirs[i] + "\\";
			}
			std::string name = Path::getInstance().getFileNameWithoutExtension(dirs[i]);
			ImGui::TextWrapped(name.c_str());
			ImGui::NextColumn();
		}
		// show all files
		auto items = Path::getInstance().getAllFilesInDirectory(m_current_path);
		for (uint32_t i = 0; i < items.size(); i++)
		{
			auto icon_name = selectIcon(items[i]);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::ImageButton(m_texture_name_to_ubo_descriptor_set[icon_name]->getHandle(), ImVec2(m_browser_button_size, m_browser_button_size), { 0, 0 }, { 1, 1 });
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				LOG_INFO("Click!");
			}
			std::string name = Path::getInstance().getFileName(items[i]);
			ImGui::TextWrapped(name.c_str());
			ImGui::NextColumn();
		}
		
		ImGui::End();
	}
	void RenderUI::drawDetail()
	{
		ImGui::Begin("Detail");
		ImGui::Text("Camera Info");
		ImGui::InputFloat3("Camera Position", m_ui_context.camera_pos.data());
		ImGui::InputFloat3("Camera Rotation", m_ui_context.camera_rotation.data());
		ImGui::InputFloat("Camera Speed", &m_ui_context.camera_speed);
		ImGui::Separator();
		ImGui::Text("Cube Info");
		ImGui::InputFloat3("Cube Position", m_ui_context.cube_pos.data());
		ImGui::InputFloat3("Cube Rotation", m_ui_context.cube_rotation.data());
		ImGui::Separator();
		ImGui::Text("Debug Tools");
		std::string button_name = m_ui_context.enable_gizmos ? "Gizmos ON" : "Gizmos OFF";
		if (ImGui::Button(button_name.c_str()))
		{
			m_ui_context.enable_gizmos = !m_ui_context.enable_gizmos;
		}
		ImGui::End();
	}
	

	void RenderUI::setStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.FrameRounding = 3;
		style.GrabRounding = 3;
		style.WindowRounding = 0.0f;
		ImVec4* colors = style.Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.29f, 0.29f, 0.29f, 0.91f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	std::string RenderUI::selectIcon(const std::string& file_name)
	{
		std::string extension = Path::getInstance().getFileExtension(file_name);
		std::string icon_name = "file";
		if (extension == ".png" || extension == ".jpg")
		{
			icon_name = "image";
		}
		else if (extension == ".material")
		{
			icon_name = "eye";
		}
		else if (extension == ".scene")
		{
			icon_name = "location";
		}
		else if (extension == ".bin")
		{
			icon_name = "Field-Binary";
		}
		else if (extension == ".gltf")
		{
			icon_name = "CodeSandbox";
		}
		else if (extension == ".ttf")
		{
			icon_name = "font-size";
		}
		else if (extension == ".ini")
		{
			icon_name = "setting";
		}
		
		return icon_name;
	}
	void RenderUI::tick(RHICommandBuffer& cmd, uint32_t frame_index)
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		drawMainMenuBar();
		drawHierarchy();
		drawScene();
		drawBrowser();
		drawDetail();
		
		ImGui::Render();

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd.getHandle(frame_index));
	}
}