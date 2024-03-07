#pragma once

#include "Marco.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIDescriptorPool.h"
#include "RHI/Public/RHIVertexBuffer.h"
#include "RHI/Public/RHIIndexBuffer.h"
#include "RHI/Public/RHIRenderPass.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "Render/Pipeline/SkyboxPipeline.h"

namespace ToolEngine
{
	class RenderSkybox
	{
	public:
		RenderSkybox(RHIDevice& device, RHIRenderPass& render_pass, RHIDescriptorPool& pool);
		~RenderSkybox();

		void init(RHIUniformBuffer& ubo, RHITextureImage& default_image, RHITextureCube& skybox_texture);
		void tick(RHICommandBuffer& cmd, uint32_t frame_index);
	private:
		RHIDevice& m_device;
		RHIDescriptorPool& m_ubo_descriptor_pool;
		std::unique_ptr<SkyboxPipeline> m_skybox_pipeline;

		std::unique_ptr<RHIVertexBuffer> m_skybox_vertex_buffer;
		std::unique_ptr<RHIIndexBuffer> m_skybox_index_buffer;
		std::unique_ptr<RHIDescriptorSet> m_descriptor_set;
	};
}