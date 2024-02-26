#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIDescriptorPool.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "Geometry/RenderScene.h"
#include "RHI/Public/RHIVertexBuffer.h"
#include "RHI/Public/RHIIndexBuffer.h"
#include "RHI/Public/RHIUniformBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "RHI/Public/RHITextureImage.h"
#include "Render/Pipeline/GizmosPipeline.h"
#include "Geometry/UBO.h"


namespace ToolEngine
{
	class RenderGizmos
	{
	public:
		RenderGizmos(RHIDevice& device, RHIRenderPass& render_pass, RHIDescriptorPool& pool);
		~RenderGizmos();

		void tick(RHICommandBuffer& cmd, uint32_t frame_index, Camera& camera);	// TODO: replace aspect with camera
	private:
		RHIDevice& m_device;
		RHIDescriptorPool& m_ubo_descriptor_pool;

		std::vector<uint32_t> m_index_count_list;
		std::vector<std::unique_ptr<RHIVertexBuffer>> m_vertex_buffer_list;
		std::vector<std::unique_ptr<RHIIndexBuffer>> m_index_buffer_list;
		std::unique_ptr<RHIUniformBuffer> m_global_uniform_buffer;
		std::vector<std::unique_ptr<RHIDescriptorSet>> m_ubo_descriptor_set_list;
		std::vector<Transform> m_transform_list;

		std::unique_ptr<GizmosPipeline> m_gizmos_pipeline;
		PushConstant m_push_constant;

		void addMesh(Mesh& mesh, Transform& transform);
	};
}