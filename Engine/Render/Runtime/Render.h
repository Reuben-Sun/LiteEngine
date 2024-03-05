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
#include "Render/Runtime/Feature/Culling/CullingResult.h"
#include "Render/Pass/BlitPass.h"
#include "Render/Pipeline/BlitPipeline.h"
#include "Render/Runtime/Feature/Gizmos/RenderGizmos.h"
#include "Render/Editor/EditorUI.h"

namespace ToolEngine
{
	class Renderer
	{
	public:
		Renderer(RHIContext& rhi_context, UIContext& ui_context);
		~Renderer();

		void record(RenderScene& scene, RHICommandBuffer& cmd, uint32_t frame_index);
		void resize();

		uint32_t m_forward_pass_width;
		uint32_t m_forward_pass_height;

		std::unique_ptr<RHIDescriptorSet> m_scene_descriptor_set;
	private:
		RHIContext& m_rhi_context;
		UIContext& m_ui_context;
		
		std::unique_ptr<CullingResult> m_culling_result;
		std::unique_ptr<ForwardPass> m_forward_pass;
		std::unique_ptr<ForwardPipeline> m_forward_pipeline;
		std::unique_ptr<ColorResources> m_color_resources;
		std::unique_ptr<DepthResources> m_depth_resources;
		std::unique_ptr<RHIFrameBuffer> m_forward_frame_buffer;
		
		std::unique_ptr<RenderGizmos> m_render_gizmos;

		std::unique_ptr<RHIDescriptorSetLayout> m_scene_descriptor_set_layout;
		
	};
}