#include "RenderGizmos.h"
#include "Core/Time/Time.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ToolEngine
{
    RenderGizmos::RenderGizmos(RHIDevice& device, RHIRenderPass& render_pass, RHIDescriptorPool& pool)
        : m_device(device), m_ubo_descriptor_pool(pool)
    {
        m_gizmos_pipeline = std::make_unique<GizmosPipeline>(m_device, render_pass.getHandle());

        Mesh mesh = Mesh::createLine(2);
        Transform transform;
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.rotation = Quaternion::Identity();
        transform.scale = glm::vec3(2.0f, 2.0f, 2.0f);

		m_index_count_list.push_back(mesh.index_buffer.size());
        m_vertex_buffer_list.push_back(std::make_unique<RHIVertexBuffer>(m_device, mesh.vertex_buffer));
        m_index_buffer_list.push_back(std::make_unique<RHIIndexBuffer>(m_device, mesh.index_buffer));
        m_uniform_buffer_list.push_back(std::make_unique<RHIUniformBuffer>(m_device));
        m_ubo_descriptor_set_list.push_back(std::make_unique<RHIDescriptorSet>(m_device, m_ubo_descriptor_pool, m_gizmos_pipeline->getDescriptorSetLayout()));
        m_ubo_descriptor_set_list[0]->updateUniformBuffer(*m_uniform_buffer_list[0], 0);
        m_transform_list.push_back(transform);
    }
    RenderGizmos::~RenderGizmos()
    {
		m_index_count_list.clear();
		m_vertex_buffer_list.clear();
        m_index_buffer_list.clear();
        m_uniform_buffer_list.clear();
        m_ubo_descriptor_set_list.clear();
        m_transform_list.clear();
    }
    void RenderGizmos::tick(RHICommandBuffer& cmd, uint32_t frame_index, float aspect)
    {
        cmd.bindPipeline(frame_index, m_gizmos_pipeline->getHandle());
		for (int i = 0; i < m_index_count_list.size(); i++)
		{
			uint32_t index_count = m_index_count_list[i];
			VkDeviceSize offsets[] = { 0 };
			cmd.bindIndexBuffer(frame_index, *m_index_buffer_list[i], 0, VK_INDEX_TYPE_UINT32);
			cmd.bindVertexBuffer(frame_index, *m_vertex_buffer_list[i], offsets, 0, 1);

			GlobalUBO ubo{};			
			Transform& transform = m_transform_list[i];
			ubo.model_matrix = transform.getModelMatrix();
			ubo.view_matrix = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			ubo.projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 10.0f);
			ubo.projection_matrix[1][1] *= -1;

			RHIUniformBuffer& uniform_buffer = *m_uniform_buffer_list[i];
			uniform_buffer.updateBuffer(ubo);
			const std::vector<VkDescriptorSet> descriptorsets = { m_ubo_descriptor_set_list[i]->getHandle() };
			cmd.bindDescriptorSets(frame_index, VK_PIPELINE_BIND_POINT_GRAPHICS, m_gizmos_pipeline->getLayout(), descriptorsets, 0, 1);

			// draw
			cmd.drawIndexed(frame_index, index_count, 1, 0, 0, 0);
		}
    }
}