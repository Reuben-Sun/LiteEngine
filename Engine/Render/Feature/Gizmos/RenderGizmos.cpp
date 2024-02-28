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
        m_global_uniform_buffer = std::make_unique<RHIUniformBuffer>(m_device, sizeof(GlobalUBO));
        Mesh line = Mesh::createLine(5, { 0.3f, 0, 0 });
        m_mesh_name_to_index_count["line"] = line.index_buffer.size();
        m_mesh_name_to_index_buffer["line"] = std::make_unique<RHIIndexBuffer>(m_device, line.index_buffer);
        m_mesh_name_to_vertex_buffer["line"] = std::make_unique<RHIVertexBuffer>(m_device, line.vertex_buffer);
        m_mesh_name_to_descriptor_set["line"] = std::make_unique<RHIDescriptorSet>(m_device, m_ubo_descriptor_pool, m_gizmos_pipeline->getDescriptorSetLayout());
        m_mesh_name_to_descriptor_set["line"]->updateUniformBuffer(*m_global_uniform_buffer, 0);


        Transform transform;
        transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform.rotation = Quaternion::Identity();
        transform.scale = glm::vec3(2.0f, 2.0f, 2.0f);
        for (int i = -4; i <= 4; i++)
        {
            transform.position = glm::vec3(0.0f, i, 0.0f);
            GizmoObject gizmo_object;
            gizmo_object.mesh_name = "line";
            gizmo_object.transform = transform;
            gizmo_object.constant.color = glm::vec3(1.0f, 0.0f, 0.0f);
            m_gizmo_global_objects.push_back(gizmo_object);
        }
        
        Transform transform2;
        transform2.position = glm::vec3(0.0f, 0.0f, 0.0f);
        transform2.rotation = Quaternion::fromRotationZ(PI * 0.5);
        transform2.scale = glm::vec3(2.0f, 2.0f, 2.0f);
        for (int i = -4; i <= 4; i++)
        {
            transform2.position = glm::vec3(i, 0.0f, 0.0f);
            GizmoObject gizmo_object;
            gizmo_object.mesh_name = "line";
            gizmo_object.transform = transform2;
            gizmo_object.constant.color = glm::vec3(0.0f, 1.0f, 0.0f);
            m_gizmo_global_objects.push_back(gizmo_object);
        }
    }
    RenderGizmos::~RenderGizmos()
    {
    }
    void RenderGizmos::tick(RHICommandBuffer& cmd, uint32_t frame_index, Camera& camera)
    {
        cmd.bindPipeline(frame_index, m_gizmos_pipeline->getHandle());
        GlobalUBO ubo{};
        ubo.view_matrix = camera.getViewMatrix();
        ubo.projection_matrix = camera.getProjectionMatrix();
        m_global_uniform_buffer->updateBuffer(&ubo);

        for (auto& object : m_gizmo_global_objects)
        {
            drawGizmoObject(cmd, frame_index, object);
        }
        for (auto& object : m_gizmo_temp_objects)
        {
            drawGizmoObject(cmd, frame_index, object);
        }
    }
    void RenderGizmos::drawGizmoObject(RHICommandBuffer& cmd, uint32_t frame_index, GizmoObject& object)
    {
        auto mesh_name = object.mesh_name;
        cmd.bindIndexBuffer(frame_index, *m_mesh_name_to_index_buffer[mesh_name], 0, VK_INDEX_TYPE_UINT32);
        VkDeviceSize offsets[] = { 0 };
        cmd.bindVertexBuffer(frame_index, *m_mesh_name_to_vertex_buffer[mesh_name], offsets, 0, 1);
        object.constant.model_matrix = object.transform.getModelMatrix();
        cmd.pushConstants(frame_index, m_gizmos_pipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GizmoPushConstant), &object.constant);
        const std::vector<VkDescriptorSet> descriptorsets = { m_mesh_name_to_descriptor_set[mesh_name]->getHandle() };
        cmd.bindDescriptorSets(frame_index, VK_PIPELINE_BIND_POINT_GRAPHICS, m_gizmos_pipeline->getLayout(), descriptorsets, 0, 1);
        cmd.drawIndexed(frame_index, m_mesh_name_to_index_count[mesh_name], 1, 0, 0, 0);
    }
}