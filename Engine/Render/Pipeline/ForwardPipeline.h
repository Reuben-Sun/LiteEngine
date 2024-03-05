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
#include "Geometry/PushConstant.h"
#include "RHI/Public/RHIVertexBuffer.h"
#include "RHI/Public/RHIIndexBuffer.h"
#include "RHI/Public/RHIUniformBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "RHI/Public/RHIDescriptorPool.h"
#include "Render/Runtime/Feature/Culling/CullingResult.h"


namespace ToolEngine
{
	class ForwardPipeline final
	{
	public:
		ForwardPipeline(RHIDevice& device, VkRenderPass render_pass);
		~ForwardPipeline();

		RHIDescriptorSetLayout& getDescriptorSetLayout() const { return *m_ubo_descriptor_set_layout; }
		
		VkPipeline getHandle() const { return m_pipeline->getHandle(); }
		VkPipelineLayout getLayout() const { return m_pipeline_layout->getHandle(); }
		
	private:
		RHIDevice& m_device;
		VkRenderPass m_render_pass;

		std::unique_ptr<RHIPipelineLayout> m_pipeline_layout;
		std::unique_ptr<RHIPipeline> m_pipeline;
		std::unique_ptr<RHIDescriptorSetLayout> m_ubo_descriptor_set_layout;

		void createPipeline();
	};
}