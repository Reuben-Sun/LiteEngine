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
#include "Core/Path/Path.h"


namespace ToolEngine
{
	struct GizmoObject
	{
		std::string mesh_name;
		Transform transform;
		GizmoPushConstant constant;
	};
	class RenderGizmos
	{
	public:
		RenderGizmos(RHIDevice& device, RHIRenderPass& render_pass, RHIDescriptorPool& pool);
		~RenderGizmos();

		void processRenderScene(RenderScene& scene);
		void tick(RHICommandBuffer& cmd, uint32_t frame_index, Camera& camera);
	private:
		RHIDevice& m_device;
		RHIDescriptorPool& m_ubo_descriptor_pool;
		std::unique_ptr<GizmosPipeline> m_gizmos_pipeline;

		std::vector<GizmoObject> m_gizmo_global_objects;
		std::vector<GizmoObject> m_gizmo_temp_objects;
		std::unordered_map<std::string, std::unique_ptr<RHIVertexBuffer>> m_mesh_name_to_vertex_buffer;
		std::unordered_map<std::string, std::unique_ptr<RHIIndexBuffer>> m_mesh_name_to_index_buffer;
		std::unique_ptr<RHIUniformBuffer> m_global_uniform_buffer;
		std::unique_ptr<RHIDescriptorSet> m_global_descriptor_set;

		void drawGizmoObject(RHICommandBuffer& cmd, uint32_t frame_index, GizmoObject& object);
	};
}