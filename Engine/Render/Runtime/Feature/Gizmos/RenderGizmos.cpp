#include "RenderGizmos.h"
#include "Core/Time/Time.h"
#include "Geometry/GltfLoader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ToolEngine
{
    RenderGizmos::RenderGizmos(RHIDevice& device, RHIRenderPass& render_pass, RHIDescriptorPool& pool)
        : m_device(device), m_ubo_descriptor_pool(pool)
    {
        m_gizmos_pipeline = std::make_unique<GizmosPipeline>(m_device, render_pass.getHandle());
        m_global_uniform_buffer = std::make_unique<RHIUniformBuffer>(m_device, sizeof(GlobalUBO));
        m_global_descriptor_set = std::make_unique<RHIDescriptorSet>(m_device, m_ubo_descriptor_pool, m_gizmos_pipeline->getDescriptorSetLayout());
        m_global_descriptor_set->updateUniformBuffer(*m_global_uniform_buffer, 0);
        Mesh line = Mesh::createLine(5, { 0.3f, 0, 0 });
        m_mesh_name_to_index_count["line"] = line.meshs[0].index_buffer.size();
        m_mesh_name_to_index_buffer["line"] = std::make_unique<RHIIndexBuffer>(m_device, line.meshs[0].index_buffer);
        m_mesh_name_to_vertex_buffer["line"] = std::make_unique<RHIVertexBuffer>(m_device, line.meshs[0].vertex_buffer);

        auto cube_path = Path::getInstance().getAssetPath() + "Models\\Cube.gltf";
        std::unique_ptr<GltfLoader> loader = std::make_unique<GltfLoader>(cube_path);
        m_mesh_name_to_index_count["cube"] = loader->loaded_index_buffer[0].size();
        m_mesh_name_to_index_buffer["cube"] = std::make_unique<RHIIndexBuffer>(m_device, loader->loaded_index_buffer[0]);
        m_mesh_name_to_vertex_buffer["cube"] = std::make_unique<RHIVertexBuffer>(m_device, loader->loaded_vertex_buffer[0]);

        auto sphere_path = Path::getInstance().getAssetPath() + "Models\\SimpleSphere.gltf";
        std::unique_ptr<GltfLoader> loader2 = std::make_unique<GltfLoader>(sphere_path);
        m_mesh_name_to_index_count["sphere"] = loader2->loaded_index_buffer[0].size();
        m_mesh_name_to_index_buffer["sphere"] = std::make_unique<RHIIndexBuffer>(m_device, loader2->loaded_index_buffer[0]);
        m_mesh_name_to_vertex_buffer["sphere"] = std::make_unique<RHIVertexBuffer>(m_device, loader2->loaded_vertex_buffer[0]);

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
    void RenderGizmos::processRenderScene(RenderScene& scene)
    {
        m_gizmo_temp_objects.clear();
        // show physics bounding
        for (int i = 0; i < scene.render_entities.size(); i++)
        {
            auto transform = scene.render_entities[i].transform;
            auto bounding = scene.render_entities[i].bounding;
            if (bounding.type == BoundingType::Box)
            {
                GizmoObject gizmo_object;
                gizmo_object.mesh_name = "cube";
                gizmo_object.transform.position = bounding.position + transform.position;
                gizmo_object.transform.rotation = Quaternion::Identity();
                gizmo_object.transform.scale = bounding.data;
                gizmo_object.constant.color = glm::vec3(0.3f, 0.3f, 1.0f);
                m_gizmo_temp_objects.push_back(gizmo_object);
            }
            else if (bounding.type == BoundingType::Sphere)
            {
				GizmoObject gizmo_object;
				gizmo_object.mesh_name = "sphere";
				gizmo_object.transform.position = bounding.position + transform.position;
				gizmo_object.transform.rotation = Quaternion::Identity();
				gizmo_object.transform.scale = bounding.data;
				gizmo_object.constant.color = glm::vec3(0.3f, 0.3f, 1.0f);
				m_gizmo_temp_objects.push_back(gizmo_object);
            }
			
        }
    }
    void RenderGizmos::tick(RHICommandBuffer& cmd, uint32_t frame_index, Camera& camera)
    {
        cmd.beginDebugUtilsLabel(frame_index, "Gizmos Pass");
        cmd.bindPipeline(frame_index, m_gizmos_pipeline->getHandle());
        GlobalUBO ubo{};
        ubo.view_matrix = camera.getViewMatrix();
        ubo.projection_matrix = camera.getProjectionMatrix();
        m_global_uniform_buffer->updateBuffer(&ubo);
        const std::vector<VkDescriptorSet> descriptorsets = { m_global_descriptor_set->getHandle() };
        cmd.bindDescriptorSets(frame_index, VK_PIPELINE_BIND_POINT_GRAPHICS, m_gizmos_pipeline->getLayout(), descriptorsets, 0, 1);
        for (auto& object : m_gizmo_global_objects)
        {
            drawGizmoObject(cmd, frame_index, object);
        }
        for (auto& object : m_gizmo_temp_objects)
        {
            drawGizmoObject(cmd, frame_index, object);
        }
        cmd.endDebugUtilsLabel(frame_index);
    }
    void RenderGizmos::drawGizmoObject(RHICommandBuffer& cmd, uint32_t frame_index, GizmoObject& object)
    {
        auto mesh_name = object.mesh_name;
        cmd.bindIndexBuffer(frame_index, *m_mesh_name_to_index_buffer[mesh_name], 0, VK_INDEX_TYPE_UINT32);
        VkDeviceSize offsets[] = { 0 };
        cmd.bindVertexBuffer(frame_index, *m_mesh_name_to_vertex_buffer[mesh_name], offsets, 0, 1);
        object.constant.model_matrix = object.transform.getModelMatrix();
        cmd.pushConstants(frame_index, m_gizmos_pipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GizmoPushConstant), &object.constant);
        cmd.drawIndexed(frame_index, m_mesh_name_to_index_count[mesh_name], 1, 0, 0, 0);
    }
}