#pragma once

#include "Marco.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHISwapchain.h"
#include "RHI/Public/RHIPipeline.h"
#include "RHI/Public/RHIPipelineLayout.h"
#include "RHI/Public/RHIPipeline.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIFrameBuffer.h"
#include "Render/Pass/ForwardPass.h"
#include "Geometry/RenderScene.h"
#include "RHI/Public/RHIVertexBuffer.h"
#include "RHI/Public/RHIIndexBuffer.h"
#include "RHI/Public/RHIUniformBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "RHI/Public/RHIDescriptorPool.h"
#include "Render/CullingResult.h"


namespace ToolEngine
{
	class ForwardPipeline final
	{
	public:
		ForwardPipeline(RHIDevice& device, RHISwapchain& swapchain, RHIDescriptorPool& pool, uint32_t frames_count);
		~ForwardPipeline();

		RHIRenderPass& getRenderPass() const { return *m_forward_pass; }

		void tick(RHICommandBuffer& cmd, RHIFrameBuffer& frame_buffer, uint32_t frame_index, RenderScene& scene);
	private:
		RHIDevice& m_device;
		RHISwapchain& m_swapchain;
		RHIDescriptorPool& m_descriptor_pool;
		uint32_t m_frames_count;

		std::unique_ptr<RHIPipelineLayout> m_pipeline_layout;
		std::unique_ptr<ForwardPass> m_forward_pass;
		std::unique_ptr<RHIPipeline> m_pipeline;
		std::unique_ptr<RHIUniformBuffer> m_uniform_buffer;
		std::unique_ptr<RHIDescriptorSetLayout> m_ubo_descriptor_set_layout;
		std::unique_ptr<RHIDescriptorSet> m_ubo_descriptor_set;
		std::unique_ptr<CullingResult> m_culling_result;

		void createPipeline();
	};
}