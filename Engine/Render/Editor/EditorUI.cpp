#include "EditorUI.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
	EditorUI::EditorUI(RHIContext& rhi_context, UIContext& ui_context, RenderScene& scene)
		: m_rhi_context(rhi_context), m_ui_context(ui_context), m_scene(scene)
	{
		uint32_t width = m_rhi_context.m_swapchain->getWidth();
		uint32_t height = m_rhi_context.m_swapchain->getHeight();
		VkFormat color_format = m_rhi_context.m_swapchain->getFormat();
		m_ui_pass = std::make_unique<UIPass>(*m_rhi_context.m_device, color_format);
		uint32_t image_count = m_rhi_context.m_swapchain->getImageCount();
		for (uint32_t i = 0; i < image_count; i++)
		{
			m_ui_frame_buffers.emplace_back(std::make_unique<RHIFrameBuffer>(*m_rhi_context.m_device,
				m_ui_pass->getHandle(),
				m_rhi_context.m_swapchain->getImageView(i),
				width, height));
		}
		initImGui();
		setImGuiStyle();
	}
	EditorUI::~EditorUI()
	{
	}
	void EditorUI::record(RHICommandBuffer& cmd, uint32_t frame_index, RHIDescriptorSet& scene_image)
	{
		uint32_t width = m_rhi_context.m_swapchain->getWidth();
		uint32_t height = m_rhi_context.m_swapchain->getHeight();
		cmd.beginDebugUtilsLabel(frame_index, "UI Pass");
		cmd.beginRenderPass(frame_index, *m_ui_pass, *m_ui_frame_buffers[frame_index], width, height);
		cmd.setViewport(frame_index, 0, width, 0, height, 0.0f, 1.0f, 0, 1);
		cmd.setScissor(frame_index, 0, width, 0, height, 0, 1);
		
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		if (m_full_screen)
		{
			drawScene(scene_image);
		}
		else 
		{
			drawMainMenuBar();
			drawHierarchy();
			drawScene(scene_image);
			drawBrowser();
			drawDetail();
		}

		ImGui::Render();

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd.getHandle(frame_index));

		cmd.endRenderPass(frame_index);
		cmd.endDebugUtilsLabel(frame_index);
	}
	void EditorUI::initImGui()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		std::string ttf_path = Path::getInstance().getAssetPath() + "MiSans-Medium.ttf";
		io.Fonts->AddFontFromFileTTF(ttf_path.c_str(), 16.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
		std::string ini_path = Path::getInstance().getAssetPath() + "imgui.ini";
		ImGui::LoadIniSettingsFromDisk(ini_path.c_str());
		setImGuiStyle();

		ImGui_ImplGlfw_InitForVulkan(m_rhi_context.m_window.getHandle(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = m_rhi_context.m_instance->getHandle();
		init_info.PhysicalDevice = m_rhi_context.m_device->getPhysicalDevice();
		init_info.Device = m_rhi_context.m_device->getLogicalDevice();
		init_info.QueueFamily = m_rhi_context.m_device->getGraphicsFamilyIndex();
		init_info.Queue = m_rhi_context.m_device->getGraphicsQueue();
		init_info.DescriptorPool = m_rhi_context.m_descriptor_pool->getHandle();
		init_info.MinImageCount = m_rhi_context.m_swapchain->getImageCount();	// 3
		init_info.ImageCount = m_rhi_context.m_swapchain->getImageCount();	// 3;
		ImGui_ImplVulkan_Init(&init_info, m_ui_pass->getHandle());

		std::vector<RHIDescriptorType> layout_descriptor;
		layout_descriptor.push_back(RHIDescriptorType::Sampler);
		m_texture_descriptor_set_layout = std::make_unique<RHIDescriptorSetLayout>(*m_rhi_context.m_device, layout_descriptor);
		std::string icon_path = Path::getInstance().getAssetPath() + "Icon\\";
		auto icons = Path::getInstance().getAllFilesInDirectory(icon_path);
		for (auto& icon : icons)
		{
			auto name = Path::getInstance().getFileNameWithoutExtension(icon);
			m_texture_name_to_image[name] = std::make_unique<RHITextureImage>(*m_rhi_context.m_device, icon);
			m_texture_name_to_descriptor_set[name] = std::make_unique<RHIDescriptorSet>(*m_rhi_context.m_device,
				*m_rhi_context.m_descriptor_pool, *m_texture_descriptor_set_layout);
			m_texture_name_to_descriptor_set[name]->updateTextureImage(m_texture_name_to_image[name]->m_descriptor, 0, RHIDescriptorType::Sampler);
		}
		m_current_path = Path::getInstance().getAssetPath();
	}
	void EditorUI::setImGuiStyle()
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

	void EditorUI::drawMainMenuBar()
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
	void EditorUI::drawHierarchy()
	{
		ImGui::Begin("Hierarchy");

		for (int i = 0; i < m_scene.render_entities.size(); i++)
		{
			ImGuiTreeNodeFlags flags = (m_scene.m_selecting_index == i) ? ImGuiTreeNodeFlags_Bullet : ImGuiTreeNodeFlags_Leaf;
			bool open = ImGui::TreeNodeEx(m_scene.render_entities[i].mesh_name.c_str(), flags);
			if (ImGui::IsItemClicked())
			{
				m_scene.m_selecting_index = i;
			}
			if (open)
			{
				ImGui::TreePop();
			}
		}

		ImGui::End();
	}
	void EditorUI::drawScene(RHIDescriptorSet& scene_image)
	{
		ImGui::Begin("SceneView");
		ImVec2 window_size = ImGui::GetContentRegionAvail();
		// save scene bounding
		ImVec2 window_begin = ImGui::GetWindowPos();
		ImVec2 scene_begin = ImGui::GetWindowContentRegionMin();
		m_scene_bounding.x = window_begin.x + scene_begin.x;
		m_scene_bounding.y = window_begin.y + scene_begin.y;
		m_scene_bounding.z = window_begin.x + scene_begin.x + window_size.x;
		m_scene_bounding.w = window_begin.y + scene_begin.y + window_size.y;
		// scene size changed event
		if (m_ui_context.m_scene_width != (uint32_t)window_size.x || 
			m_ui_context.m_scene_height != (uint32_t)window_size.y)
		{
			m_ui_context.m_scene_width = (uint32_t)window_size.x;
			m_ui_context.m_scene_height = (uint32_t)window_size.y;
			m_ui_context.need_resize = true;
		}
		// show scene image
		ImGui::Image(scene_image.getHandle(), window_size);
		ImGui::End();
	}
	void EditorUI::drawBrowser()
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
			ImGui::ImageButton(m_texture_name_to_descriptor_set["rollback"]->getHandle(), ImVec2(m_browser_button_size, m_browser_button_size), { 0, 0 }, { 1, 1 });
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
			ImGui::ImageButton(m_texture_name_to_descriptor_set["folder-open-fill"]->getHandle(), ImVec2(m_browser_button_size, m_browser_button_size), { 0, 0 }, { 1, 1 });
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
			std::string name = Path::getInstance().getFileName(items[i]);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::ImageButton(m_texture_name_to_descriptor_set[icon_name]->getHandle(), ImVec2(m_browser_button_size, m_browser_button_size), { 0, 0 }, { 1, 1 });
			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				std::string extension = Path::getInstance().getFileExtension(items[i]);
				if (extension == ".scene")
				{
					UIReloadSceneEvent event(std::format("Scene\\{0}", name));
					m_event_callback(event);
				}
				LOG_INFO("Click!");
			}
			ImGui::TextWrapped(name.c_str());
			ImGui::NextColumn();
		}

		ImGui::End();
	}
	void EditorUI::drawDetail()
	{
		ImGui::Begin("Detail");

		ImGui::Text("Detail Info");
		ImGui::Text(m_rhi_context.m_device->getGPUInfo().deviceName);
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::Separator();

		ImGui::Text("Camera Info");
		std::vector<float> camera_position = { m_scene.camera.transform.position.x, m_scene.camera.transform.position.y, m_scene.camera.transform.position.z };
		auto camera_euler = m_scene.camera.transform.rotation.getEulerDegrees();
		std::vector<float> camera_rotation = { camera_euler[0], camera_euler[1], camera_euler[2] };
		ImGui::InputFloat3("Camera Position", camera_position.data());
		ImGui::InputFloat3("Camera Rotation", camera_rotation.data());
		ImGui::InputFloat("Camera Speed", &m_scene.camera.camera_speed);
		ImGui::Separator();

		ImGui::Text("Debug Tools");
		std::string button_name = m_ui_context.enable_gizmos ? "Gizmos ON" : "Gizmos OFF";
		if (ImGui::Button(button_name.c_str()))
		{
			m_ui_context.enable_gizmos = !m_ui_context.enable_gizmos;
		}
		static const char* items[] = { "Shadered", "NormalWS", "NormalWS Mapped", "Metallic", "Roughness", "Emission", "EnvReflection"};
		ImGui::Combo("Debug Mode", &m_ui_context.debug_mode, items, IM_ARRAYSIZE(items));
		ImGui::SliderFloat("Outline Width", &m_ui_context.outline_width, 0.0f, 0.1f);
		ImGui::Separator();
		
		auto& entity = m_scene.render_entities[m_scene.m_selecting_index];
		ImGui::Text("Render Entity: %s", entity.mesh_name.c_str());
		std::vector<float> entity_position = { entity.transform.position.x, entity.transform.position.y, entity.transform.position.z};
		auto entity_euler = entity.transform.rotation.getEulerDegrees();
		std::vector<float> entity_rotation = { entity_euler[0], entity_euler[1], entity_euler[2] };
		std::vector<float> entity_scale = { entity.transform.scale.x, entity.transform.scale.y, entity.transform.scale.z };
		ImGui::Text("Transform");
		ImGui::InputFloat3("Position", entity_position.data());
		ImGui::InputFloat3("Rotation", entity_rotation.data());
		ImGui::InputFloat3("Scale", entity_scale.data());
		
		ImGui::Text("Materials");
		for (int material_index = 0; material_index < entity.material_names.size(); material_index++)
		{
			auto material_name = entity.material_names[material_index];
			ImGui::Text(material_name.c_str());
			PushConstant& push_constant = m_scene.resources->m_material_name_to_push_constant[material_name];
			std::string base_color_name = std::format("BaseColor {0}", material_index);
			std::string metallic_slider_name = std::format("Metallic {0}", material_index);
			std::string roughness_slider_name = std::format("Roughness {0}", material_index);
			std::string emission_color_name = std::format("EmissionColor {0}", material_index);
			std::vector<float> base_color = { push_constant.base_color[0], push_constant.base_color[1], push_constant.base_color[2] };
			ImGui::ColorEdit3(base_color_name.c_str(), base_color.data());
			push_constant.base_color = { base_color[0], base_color[1], base_color[2] };
			ImGui::SliderFloat(metallic_slider_name.c_str(), &push_constant.metallic, 0.0f, 1.0f);
			ImGui::SliderFloat(roughness_slider_name.c_str(), &push_constant.roughness, 0.0f, 1.0f);
			std::vector<float> emission_color = { push_constant.emission_color[0], push_constant.emission_color[1], push_constant.emission_color[2] };
			ImGui::ColorEdit3(emission_color_name.c_str(), emission_color.data());
			push_constant.emission_color = { emission_color[0], emission_color[1], emission_color[2] };
		}
		ImGui::End();
	}
	std::string EditorUI::selectIcon(const std::string& file_name)
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
}