#pragma once

#include "Marco.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIDescriptorPool.h"
#include "RHI/Public/RHIVertexBuffer.h"
#include "RHI/Public/RHIIndexBuffer.h"
#include "RHI/Public/RHIRenderPass.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "Render/Resources/RenderScene.h"
#include "Render/Resources/PushConstant.h"
#include "Render/Pipeline/OutlinePipeline.h"
#include "Geometry/Camera.h"

namespace ToolEngine
{
	class RenderOutline
	{
	public:
		RenderOutline(RHIDevice& device, RHIRenderPass& render_pass, RHIDescriptorPool& pool);
		~RenderOutline();
		void init(RHIUniformBuffer& ubo);
		void tick(RHICommandBuffer& cmd, uint32_t frame_index, RenderScene& scene, float outline_width);
	private:
		RHIDevice& m_device;
		RHIDescriptorPool& m_ubo_descriptor_pool;
		std::unique_ptr<OutlinePipeline> m_outline_pipeline;
		std::unique_ptr<RHIDescriptorSet> m_descriptor_set;
	};
}