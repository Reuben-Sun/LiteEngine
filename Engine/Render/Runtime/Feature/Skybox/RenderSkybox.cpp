#include "RenderSkybox.h"
#include "Geometry/GltfLoader.h"
#include "Core/Path/Path.h"
#include "Core/Math/Transform.h"
#include "Geometry/PushConstant.h"

namespace ToolEngine
{
	RenderSkybox::RenderSkybox(RHIDevice& device, RHIRenderPass& render_pass, RHIDescriptorPool& pool)
		: m_device(device), m_ubo_descriptor_pool(pool)
	{
		m_skybox_pipeline = std::make_unique<SkyboxPipeline>(m_device, render_pass.getHandle());
		m_descriptor_set = std::make_unique<RHIDescriptorSet>(m_device, m_ubo_descriptor_pool, m_skybox_pipeline->getDescriptorSetLayout());
		auto cube_path = Path::getInstance().getAssetPath() + "Models\\Cube.gltf";
		std::unique_ptr<GltfLoader> loader = std::make_unique<GltfLoader>(cube_path);
		m_skybox_index_buffer = std::make_unique<RHIIndexBuffer>(m_device, loader->loaded_index_buffer[0]);
		m_skybox_vertex_buffer = std::make_unique<RHIVertexBuffer>(m_device, loader->loaded_vertex_buffer[0]);
	}
	RenderSkybox::~RenderSkybox()
	{
	}
	void RenderSkybox::init(RHIUniformBuffer& ubo, RHITextureImage& default_image, RHITextureCube& skybox_texture)
	{
		m_descriptor_set->updateUniformBuffer(ubo, 0);
		//m_descriptor_set->updateTextureImage(default_image.m_descriptor, 1);
		m_descriptor_set->updateTextureImage(skybox_texture.m_descriptor, 1, RHIDescriptorType::Sampler);
	}
	void RenderSkybox::tick(RHICommandBuffer& cmd, uint32_t frame_index)
	{
		cmd.beginDebugUtilsLabel(frame_index, "Skybox Pass");
		cmd.bindPipeline(frame_index, m_skybox_pipeline->getHandle());
		const std::vector<VkDescriptorSet> descriptorsets = { m_descriptor_set->getHandle() };
		cmd.bindDescriptorSets(frame_index, VK_PIPELINE_BIND_POINT_GRAPHICS, m_skybox_pipeline->getLayout(), descriptorsets, 0, 1);
		cmd.bindIndexBuffer(frame_index, *m_skybox_index_buffer, 0, VK_INDEX_TYPE_UINT32);
		VkDeviceSize offsets[] = { 0 };
		cmd.bindVertexBuffer(frame_index, *m_skybox_vertex_buffer, offsets, 0, 1);
		Transform transform;
		transform.rotation = Quaternion::Identity();
		transform.scale = glm::vec3(100, 100, 100);
		SkyboxPushConstant constant;
		constant.model_matrix = transform.getModelMatrix();
		cmd.pushConstants(frame_index, m_skybox_pipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(SkyboxPushConstant), &constant);
		cmd.drawIndexed(frame_index, m_skybox_index_buffer->getIndexCount(), 1, 0, 0, 0);
		cmd.endDebugUtilsLabel(frame_index);
	}
}