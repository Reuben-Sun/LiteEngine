#include "Render.h"
#include "RHI/Public/RHISingleTimeCommandBuffer.h"
#include "Core/Time/Time.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ToolEngine
{
	Renderer::Renderer(RHIContext& rhi_context) : m_rhi_context(rhi_context)
	{
		LOG_INFO("Create Renderer!")
		VkFormat color_format = m_rhi_context.m_swapchain->getFormat();
		VkFormat depth_format = m_rhi_context.m_device->getDepthFormatDetail();
		uint32_t width = m_rhi_context.m_swapchain->getWidth();
		uint32_t height = m_rhi_context.m_swapchain->getHeight();

		m_depth_resources = std::make_unique<DepthResources>(*m_rhi_context.m_device, width, height);

		m_forward_pass = std::make_unique<ForwardPass>(*m_rhi_context.m_device, color_format, depth_format);

		m_forward_pipeline = std::make_unique<ForwardPipeline>(*m_rhi_context.m_device, m_forward_pass->getHandle());

		uint32_t swapchain_image_count = m_rhi_context.m_swapchain->getImageCount();
		m_max_frames_in_flight = swapchain_image_count;
		for (uint32_t i = 0; i < swapchain_image_count; i++)
		{
			m_frame_buffers.emplace_back(std::make_unique<RHIFrameBuffer>(*m_rhi_context.m_device, 
				m_forward_pass->getHandle(),
				m_rhi_context.m_swapchain->getImageView(i), 
				m_depth_resources->getImageView(), width, height));
		}

		m_command_buffer = std::make_unique<RHICommandBuffer>(*m_rhi_context.m_device, m_max_frames_in_flight);

		for (uint32_t i = 0; i < swapchain_image_count; i++)
		{
			m_in_flight_fences.emplace_back(std::make_unique<Fence>(*m_rhi_context.m_device));
			m_image_available_semaphores.emplace_back(std::make_unique<Semaphore>(*m_rhi_context.m_device));
			m_render_finished_semaphores.emplace_back(std::make_unique<Semaphore>(*m_rhi_context.m_device));
		}

		m_culling_result = std::make_unique<CullingResult>(*m_rhi_context.m_device, 
			m_forward_pipeline->getDescriptorSetLayout(), *m_rhi_context.m_descriptor_pool);

		initUI();
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::tick(RenderScene& scene)
	{
		uint32_t frame_index = getFrameIndex();

		m_in_flight_fences[frame_index]->wait();

		uint32_t image_index = m_rhi_context.m_swapchain->acquireNextTexture(*m_image_available_semaphores[frame_index]);
		
		m_culling_result->cull(scene);

		uint32_t width = m_rhi_context.m_swapchain->getWidth();
		uint32_t height = m_rhi_context.m_swapchain->getHeight();

		m_command_buffer->beginRecord(frame_index);

		m_command_buffer->beginRenderPass(frame_index, *m_forward_pass, *m_frame_buffers[frame_index], width, height);

		m_command_buffer->bindPipeline(frame_index, m_forward_pipeline->getHandle());

		m_command_buffer->setViewport(frame_index, width, height, 0.0f, 1.0f, 0, 1);

		m_command_buffer->setScissor(frame_index, width, height, 0, 1);

		// culling
		m_culling_result->cull(scene);
		// draw culling result
		for (int i = 0; i < scene.mesh_list.size(); i++)
		{
			// binding index buffer and vertex buffer
			uint32_t index_count = scene.mesh_list[i].index_buffer.size();
			RHIIndexBuffer& index_buffer = m_culling_result->getIndexBuffer(scene.mesh_name_list[i]);
			RHIVertexBuffer& vertex_buffer = m_culling_result->getVertexBuffer(scene.mesh_name_list[i]);
			VkDeviceSize offsets[] = { 0 };
			m_command_buffer->bindIndexBuffer(frame_index, index_buffer, 0, VK_INDEX_TYPE_UINT32);
			m_command_buffer->bindVertexBuffer(frame_index, vertex_buffer, offsets, 0, 1);

			// update ubo
			GlobalUBO ubo{};
			float time = Time::getInstance().getDeltaTime();
			Transform& transform = scene.mesh_transform_list[i];
			transform.rotation = Quaternion::fromRotationZ(Time::getInstance().getCurrentTime());
			ubo.model_matrix = transform.getModelMatrix();
			ubo.view_matrix = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			ubo.projection_matrix = glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 10.0f);
			ubo.projection_matrix[1][1] *= -1;

			// binding ubo
			RHIUniformBuffer& uniform_buffer = m_culling_result->getUniformBuffer(scene.mesh_name_list[i]);
			RHIDescriptorSet& descriptor_set = m_culling_result->getDescriptorSet(scene.mesh_name_list[i]);
			uniform_buffer.updateBuffer(ubo);
			const std::vector<VkDescriptorSet> descriptorsets = { descriptor_set.getHandle() };
			m_command_buffer->bindDescriptorSets(frame_index, VK_PIPELINE_BIND_POINT_GRAPHICS, m_forward_pipeline->getLayout(), descriptorsets, 0, 1);
			
			// draw
			m_command_buffer->draw(frame_index, index_count, 1, 0, 0, 0);
		}

		drawUI(*m_command_buffer, frame_index);

		m_command_buffer->endRenderPass(frame_index);

		m_command_buffer->endRecord(frame_index);

		m_in_flight_fences[frame_index]->resetFence();

		//m_command_buffer->resetCommandBuffer(frame_index);

		std::vector<VkSemaphore> wait_semaphores { m_image_available_semaphores[frame_index]->getHandle() };
		std::vector<VkSemaphore> signal_semaphores { m_render_finished_semaphores[frame_index]->getHandle() };
		m_command_buffer->submitCommandBuffer(frame_index, wait_semaphores, 
			signal_semaphores, m_in_flight_fences[frame_index]->getHandle());

		std::vector<VkSwapchainKHR> swapchains{ m_rhi_context.m_swapchain->getHandle() };
		m_rhi_context.m_device->present(signal_semaphores, image_index, swapchains);

		m_current_frame++;
	}
	void Renderer::initUI()
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForVulkan(m_rhi_context.m_window.getHandle(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = m_rhi_context.m_instance->getHandle();
		init_info.PhysicalDevice = m_rhi_context.m_device->getPhysicalDevice();
		init_info.Device = m_rhi_context.m_device->getLogicalDevice();
		init_info.QueueFamily = m_rhi_context.m_device->getGraphicsFamilyIndex();
		init_info.Queue = m_rhi_context.m_device->getGraphicsQueue();
		init_info.DescriptorPool = m_rhi_context.m_descriptor_pool->getHandle();
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		ImGui_ImplVulkan_Init(&init_info, m_forward_pass->getHandle());
		{
			std::unique_ptr<RHISingleTimeCommandBuffer> single_time_command_buffer = std::make_unique<RHISingleTimeCommandBuffer>(*m_rhi_context.m_device);
			ImGui_ImplVulkan_CreateFontsTexture(single_time_command_buffer->getHandle());
		}
		ImGui_ImplVulkan_DestroyFontUploadObjects();
	}
	void Renderer::drawUI(RHICommandBuffer& cmd, uint32_t frame_index)
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd.getHandle(frame_index));
	}
}