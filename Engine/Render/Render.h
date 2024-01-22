#pragma once

#include "Marco.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIFrameBuffer.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHISynchronization.h"
#include "Render/Pipeline/ForwardPipeline.h"
#include "Render/Resources/DepthResources.h"
#include "Geometry/RenderScene.h"
#include "CullingResult.h"
#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

namespace ToolEngine
{
	class Renderer
	{
	public:
		Renderer(RHIContext& rhi_context);
		~Renderer();

		void tick(RenderScene& scene);
		// TODO: resize frame
	private:
		RHIContext& m_rhi_context;
		uint32_t m_max_frames_in_flight{ 3 };
		std::unique_ptr<ForwardPass> m_forward_pass;
		std::unique_ptr<ForwardPipeline> m_forward_pipeline;
		std::unique_ptr<DepthResources> m_depth_resources;
		std::vector<std::unique_ptr<RHIFrameBuffer>> m_frame_buffers;
		std::unique_ptr<RHICommandBuffer> m_command_buffer;
		std::vector<std::unique_ptr<Fence>> m_in_flight_fences;
		std::vector<std::unique_ptr<Semaphore>> m_image_available_semaphores;
		std::vector<std::unique_ptr<Semaphore>> m_render_finished_semaphores;
		std::unique_ptr<CullingResult> m_culling_result;

		uint32_t m_current_frame{ 0 };
		uint32_t getFrameIndex() const { return m_current_frame % m_max_frames_in_flight; }

		void initUI();
		void drawUI(VkCommandBuffer cmd);
	};
}