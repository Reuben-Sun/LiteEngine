#pragma once

#include "Marco.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIFrameBuffer.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHISynchronization.h"
#include "Render/Pipeline/ForwardPipeline.h"
#include "Render/Pass/UIPass.h"
#include "Render/Resources/ColorResources.h"
#include "Render/Resources/DepthResources.h"
#include "Geometry/RenderScene.h"
#include "Render/Feature/Culling/CullingResult.h"
#include "Render/Feature/UI/RenderUI.h"
#include "Render/Pass/BlitPass.h"
#include "Render/Pipeline/BlitPipeline.h"
#include "Render/Feature/Gizmos/RenderGizmos.h"

namespace ToolEngine
{
	class Renderer
	{
	public:
		Renderer(RHIContext& rhi_context);
		~Renderer();

		bool m_enable_ui = true;
		void tick(RenderScene& scene);
		void resize();

		uint32_t m_forward_pass_width;
		uint32_t m_forward_pass_height;
	private:
		RHIContext& m_rhi_context;
		uint32_t m_max_frames_in_flight{ 3 };
		
		std::unique_ptr<CullingResult> m_culling_result;
		std::unique_ptr<ForwardPass> m_forward_pass;
		std::unique_ptr<ForwardPipeline> m_forward_pipeline;
		std::unique_ptr<ColorResources> m_color_resources;
		std::unique_ptr<DepthResources> m_depth_resources;
		std::unique_ptr<RHIFrameBuffer> m_forward_frame_buffer;
		
		std::unique_ptr<RenderGizmos> m_render_gizmos;

		std::unique_ptr<UIPass> m_ui_pass;
		std::vector<std::unique_ptr<RHIFrameBuffer>> m_ui_frame_buffers;
		std::unique_ptr<RenderUI> m_render_ui;

		std::unique_ptr<BlitPass> m_blit_pass;
		std::unique_ptr<BlitPipeline> m_blit_pipeline;
		std::unique_ptr<RHIDescriptorSet> m_blit_descriptor_set;
		std::vector<std::unique_ptr<RHIFrameBuffer>> m_blit_frame_buffers;

		std::unique_ptr<RHICommandBuffer> m_command_buffer;
		std::vector<std::unique_ptr<Fence>> m_in_flight_fences;
		std::vector<std::unique_ptr<Semaphore>> m_image_available_semaphores;
		std::vector<std::unique_ptr<Semaphore>> m_render_finished_semaphores;
		
		uint32_t m_current_frame{ 0 };
		uint32_t getFrameIndex() const { return m_current_frame % m_max_frames_in_flight; }

		bool prepareFrame(uint32_t& image_index);
		void submitFrame(uint32_t image_index);
		void record(RenderScene& scene);
	};
}