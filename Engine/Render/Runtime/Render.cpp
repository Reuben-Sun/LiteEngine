#include "Render.h"
#include "RHI/Public/RHISingleTimeCommandBuffer.h"
#include "Core/Time/Time.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Render/Resources/UBO.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
	Renderer::Renderer(RHIContext& rhi_context, UIContext& ui_context, RenderScene& scene) 
		: m_rhi_context(rhi_context), m_ui_context(ui_context), m_scene(scene)
	{
		LOG_INFO("Create Renderer!")
		VkFormat color_format = m_rhi_context.m_swapchain->getFormat();
		VkFormat depth_format = m_rhi_context.m_device->getDepthFormatDetail();
		uint32_t width = m_rhi_context.m_swapchain->getWidth();
		uint32_t height = m_rhi_context.m_swapchain->getHeight();
		m_forward_pass_width = width;
		m_forward_pass_height = height;

		m_color_resources = std::make_unique<ColorResources>(*m_rhi_context.m_device, width, height, color_format);
		m_depth_resources = std::make_unique<DepthResources>(*m_rhi_context.m_device, width, height);

		m_forward_pass = std::make_unique<ForwardPass>(*m_rhi_context.m_device, color_format, depth_format);
		m_forward_pipeline = std::make_unique<ForwardPipeline>(*m_rhi_context.m_device, m_forward_pass->getHandle());
		
		m_culling_result = std::make_unique<CullingResult>(*m_rhi_context.m_device,
			m_forward_pipeline->getDescriptorSetLayout(), *m_rhi_context.m_descriptor_pool);
		m_render_gizmos = std::make_unique<RenderGizmos>(*m_rhi_context.m_device, *m_forward_pass, *m_rhi_context.m_descriptor_pool);
		m_render_skybox = std::make_unique<RenderSkybox>(*m_rhi_context.m_device, *m_forward_pass, *m_rhi_context.m_descriptor_pool);
		m_render_skybox->init(*m_scene.resources->m_global_ubo, *m_scene.resources->m_global_default_texture, *m_scene.resources->m_skybox_texture);

		uint32_t swapchain_image_count = m_rhi_context.m_swapchain->getImageCount();
		m_forward_frame_buffer = std::make_unique<RHIFrameBuffer>(*m_rhi_context.m_device,
			m_forward_pass->getHandle(),
			m_color_resources->getImageView(),
			m_depth_resources->getImageView(),
			width, height);

		// scene image for imgui
		std::vector<RHIDescriptorType> layout_descriptor;
		layout_descriptor.push_back(RHIDescriptorType::Sampler);
		m_scene_descriptor_set_layout = std::make_unique<RHIDescriptorSetLayout>(*m_rhi_context.m_device, layout_descriptor);
		m_scene_descriptor_set = std::make_unique<RHIDescriptorSet>(*m_rhi_context.m_device, *m_rhi_context.m_descriptor_pool, *m_scene_descriptor_set_layout);
		m_scene_descriptor_set->updateTextureImage(m_color_resources->m_descriptor, 0, RHIDescriptorType::Sampler);
	}

	Renderer::~Renderer()
	{
	}


	// when imgui scene window resize, forward pass need resize
	void Renderer::resize()
	{
		m_rhi_context.m_device->waitIdle();
		VkFormat color_format = m_rhi_context.m_swapchain->getFormat();
		VkFormat depth_format = m_rhi_context.m_device->getDepthFormatDetail();
		uint32_t width = m_ui_context.m_scene_width;
		uint32_t height = m_ui_context.m_scene_height;

		m_forward_pass_width = width;
		m_forward_pass_height = height;

		m_color_resources = std::make_unique<ColorResources>(*m_rhi_context.m_device, width, height, color_format);
		m_depth_resources = std::make_unique<DepthResources>(*m_rhi_context.m_device, width, height);

		m_forward_frame_buffer = std::make_unique<RHIFrameBuffer>(*m_rhi_context.m_device,
			m_forward_pass->getHandle(),
			m_color_resources->getImageView(),
			m_depth_resources->getImageView(),
			width, height);
		m_scene_descriptor_set->updateTextureImage(m_color_resources->m_descriptor, 0, RHIDescriptorType::Sampler);
	}
	
	void Renderer::record(RHICommandBuffer& cmd, uint32_t frame_index)
	{
		OPTICK_EVENT();
		if (m_ui_context.need_resize)
		{
			resize();
			m_ui_context.need_resize = false;
		}
		uint32_t w_start = 0;
		uint32_t h_start = 0;
		uint32_t w_width = m_forward_pass_width;
		uint32_t h_height = m_forward_pass_height;
		cmd.beginDebugUtilsLabel(frame_index, "Forward Pass");
		cmd.beginRenderPass(frame_index, *m_forward_pass, *m_forward_frame_buffer, w_width, h_height);

		cmd.bindPipeline(frame_index, m_forward_pipeline->getHandle());

		cmd.setViewport(frame_index, w_start, w_width, h_start, h_height, 0.0f, 1.0f, 0, 1);

		cmd.setScissor(frame_index, w_start, w_width, h_start, h_height, 0, 1);

		// culling
		m_culling_result->cull(m_scene);
		OPTICK_PUSH("Draw Forward");
		// global ubo
		GlobalUBO ubo{};
		m_scene.camera.aspect = m_forward_pass_width / (float)m_forward_pass_height;
		ubo.view_matrix = m_scene.camera.getViewMatrix();
		ubo.projection_matrix = m_scene.camera.getProjectionMatrix();
		ubo.camera_position = glm::vec4(m_scene.camera.transform.position, 0.0f);
		ubo.directional_light = m_scene.resources->m_dir_light;
		m_scene.resources->m_global_ubo->updateBuffer(&ubo);
		// draw culling result
		for (int i = 0; i < m_scene.render_entities.size(); i++)
		{
			auto& entity = m_scene.render_entities[i];
			auto& sub_model_names = m_scene.resources->m_model_name_to_sub_model_name[entity.mesh_path];
			for (int sub_index = 0; sub_index < sub_model_names.size(); sub_index++)
			{
				auto& sub_model_name = sub_model_names[sub_index];
				uint32_t material_index = sub_index;
				if (material_index >= entity.material_names.size())
				{
					material_index = entity.material_names.size() - 1;
				}
				auto& material_name = entity.material_names[material_index];
				RHIIndexBuffer& index_buffer = m_scene.resources->getIndexBuffer(sub_model_name);
				RHIVertexBuffer& vertex_buffer = m_scene.resources->getVertexBuffer(sub_model_name);
				VkDeviceSize offsets[] = { 0 };
				cmd.bindIndexBuffer(frame_index, index_buffer, 0, VK_INDEX_TYPE_UINT32);
				cmd.bindVertexBuffer(frame_index, vertex_buffer, offsets, 0, 1);
				// binding texture
				RHIDescriptorSet& descriptor_set = m_culling_result->getDescriptorSet(material_name);
				const std::vector<VkDescriptorSet> descriptorsets = { descriptor_set.getHandle() };
				cmd.bindDescriptorSets(frame_index, VK_PIPELINE_BIND_POINT_GRAPHICS, m_forward_pipeline->getLayout(), descriptorsets, 0, 1);
				// push constant
				PushConstant push_constant = m_scene.resources->m_material_name_to_push_constant[material_name];
				push_constant.model_matrix = entity.transform.getModelMatrix();
				push_constant.debug_mode = m_ui_context.debug_mode;
				cmd.pushConstants(frame_index, m_forward_pipeline->getLayout(), VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstant), &push_constant);
				// draw
				cmd.drawIndexed(frame_index, index_buffer.getIndexCount(), 1, 0, 0, 0);
			}
		}
		OPTICK_POP();
		OPTICK_PUSH("Draw Gizmos");
		if (m_ui_context.enable_gizmos)
		{
			m_render_gizmos->processRenderScene(m_scene);
			m_render_gizmos->tick(cmd, frame_index, m_scene.camera);
		}
		OPTICK_POP();
		m_render_skybox->tick(cmd, frame_index, m_scene.camera);

		cmd.endRenderPass(frame_index);
		cmd.endDebugUtilsLabel(frame_index);
	}

}