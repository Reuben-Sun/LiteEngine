#include "RenderOutline.h"
#include <glm/gtc/matrix_inverse.hpp>

namespace ToolEngine
{
    RenderOutline::RenderOutline(RHIDevice& device, RHIRenderPass& render_pass, RHIDescriptorPool& pool)
        : m_device(device), m_ubo_descriptor_pool(pool)
    {
        m_outline_pipeline = std::make_unique<OutlinePipeline>(m_device, render_pass.getHandle());
        m_descriptor_set = std::make_unique<RHIDescriptorSet>(m_device, m_ubo_descriptor_pool, m_outline_pipeline->getDescriptorSetLayout());
    }
    RenderOutline::~RenderOutline()
    {
    }
    void RenderOutline::init(RHIUniformBuffer& ubo)
    {
        m_descriptor_set->updateUniformBuffer(ubo, 0);
    }
    void RenderOutline::tick(RHICommandBuffer& cmd, uint32_t frame_index, RenderScene& scene, float outline_width)
    {
        cmd.beginDebugUtilsLabel(frame_index, "Outline Pass");
        cmd.bindPipeline(frame_index, m_outline_pipeline->getHandle());
        const std::vector<VkDescriptorSet> descriptorsets = { m_descriptor_set->getHandle() };
        cmd.bindDescriptorSets(frame_index, VK_PIPELINE_BIND_POINT_GRAPHICS, m_outline_pipeline->getLayout(), descriptorsets, 0, 1);
        auto& entity = scene.render_entities[scene.m_selecting_index];
        auto& sub_model_names = scene.resources->m_model_name_to_sub_model_name[entity.mesh_path];
        for (int sub_index = 0; sub_index < sub_model_names.size(); sub_index++)
        {
            RHIIndexBuffer& index_buffer = scene.resources->getIndexBuffer(sub_model_names[sub_index]);
            RHIVertexBuffer& vertex_buffer = scene.resources->getVertexBuffer(sub_model_names[sub_index]);
            cmd.bindIndexBuffer(frame_index, index_buffer, 0, VK_INDEX_TYPE_UINT32);
            VkDeviceSize offsets[] = { 0 };
            cmd.bindVertexBuffer(frame_index, vertex_buffer, offsets, 0, 1);
            OutlinePushConstant constant;
            constant.model_matrix = entity.transform.getModelMatrix();
            constant.color = glm::vec3(1.0f, 0.9f, 0.0f);
            constant.width = outline_width;
            constant.inv_m_v_matrix = glm::inverseTranspose(scene.camera.getViewMatrix() * constant.model_matrix);
            cmd.pushConstants(frame_index, m_outline_pipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(OutlinePushConstant), &constant);
            cmd.drawIndexed(frame_index, index_buffer.getIndexCount(), 1, 0, 0, 0);
        }
        
        cmd.endDebugUtilsLabel(frame_index);
    }
}