#include "RenderContext.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
	RenderContext::RenderContext(RHIContext& rhi_context): m_rhi_context(rhi_context)
	{
		m_scene.m_resources = std::make_unique<SceneResources>(*rhi_context.m_device);
		m_editor_ui = std::make_unique<EditorUI>(m_rhi_context, m_ui_context);
		m_renderer = std::make_unique<Renderer>(m_rhi_context, m_ui_context, m_scene);
		m_max_frames_in_flight = m_rhi_context.m_swapchain->getImageCount();
		m_command_buffer = std::make_unique<RHICommandBuffer>(*m_rhi_context.m_device, m_max_frames_in_flight);
		for (uint32_t i = 0; i < m_max_frames_in_flight; i++)
		{
			m_in_flight_fences.emplace_back(std::make_unique<Fence>(*m_rhi_context.m_device));
			m_image_available_semaphores.emplace_back(std::make_unique<Semaphore>(*m_rhi_context.m_device));
			m_render_finished_semaphores.emplace_back(std::make_unique<Semaphore>(*m_rhi_context.m_device));
		}
	}
	RenderContext::~RenderContext()
	{
	}
	void RenderContext::tick(LogicScene& scene)
	{
		OPTICK_PUSH("Process Logic scene to Render scene");
		m_scene.init(scene);
		m_scene.tick();
		auto ui_info_view = scene.scene_context.view<UIInfoComponent>();
		ui_info_view.get<UIInfoComponent>(ui_info_view.front()).scene_bounding = m_editor_ui->m_scene_bounding;
		OPTICK_POP();

		OPTICK_PUSH("RenderContext tick");
		uint32_t image_index;
		if (!prepareFrame(image_index))
		{
			return;
		}
		uint32_t frame_index = getFrameIndex();
		m_command_buffer->beginRecord(frame_index);
		m_renderer->record(*m_command_buffer, frame_index);
		m_editor_ui->record(m_scene, *m_command_buffer, frame_index, *m_renderer->m_scene_descriptor_set);
		m_command_buffer->endRecord(frame_index);
		submitFrame(image_index);
		OPTICK_POP();
	}
	void RenderContext::setFullScreen()
	{
		m_editor_ui->m_full_screen = !m_editor_ui->m_full_screen;
	}
	bool RenderContext::prepareFrame(uint32_t& image_index)
	{
		OPTICK_EVENT();
		uint32_t frame_index = getFrameIndex();
		m_in_flight_fences[frame_index]->wait();

		auto result = m_rhi_context.m_swapchain->acquireNextTexture(*m_image_available_semaphores[frame_index], image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			return false;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			LOG_ERROR("failed to acquire swap chain image!");
		}

		return true;
	}
	void RenderContext::submitFrame(uint32_t image_index)
	{
		OPTICK_EVENT();
		uint32_t frame_index = getFrameIndex();
		m_in_flight_fences[frame_index]->resetFence();

		std::vector<VkSemaphore> wait_semaphores{ m_image_available_semaphores[frame_index]->getHandle() };
		std::vector<VkSemaphore> signal_semaphores{ m_render_finished_semaphores[frame_index]->getHandle() };
		m_command_buffer->submitCommandBuffer(frame_index, wait_semaphores,
			signal_semaphores, m_in_flight_fences[frame_index]->getHandle());

		std::vector<VkSwapchainKHR> swapchains{ m_rhi_context.m_swapchain->getHandle() };
		m_rhi_context.m_device->present(signal_semaphores, image_index, swapchains);

		m_current_frame++;
	}
	
}