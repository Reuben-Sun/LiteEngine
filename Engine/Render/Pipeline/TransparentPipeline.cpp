#include "TransparentPipeline.h"

namespace ToolEngine
{
	TransparentPipeline::TransparentPipeline(RHIDevice& device, VkRenderPass render_pass)
		: RenderPipeline(device, render_pass)
	{
		std::vector<RHIDescriptorType> layout_descriptor;
		layout_descriptor.push_back(RHIDescriptorType::ConstantBuffer);
		layout_descriptor.push_back(RHIDescriptorType::Sampler);
		layout_descriptor.push_back(RHIDescriptorType::TextureSRV);
		m_ubo_descriptor_set_layout = std::make_unique<RHIDescriptorSetLayout>(m_device, layout_descriptor);
		createPipeline();
		LOG_INFO("Create TransparentPipeline!");
	}
	TransparentPipeline::~TransparentPipeline()
	{
	}
	void TransparentPipeline::createPipeline()
	{
		// shader
		RHIShader vertex_shader_module(m_device, "Transparent.vert.spv");
		RHIShader fragment_shader_module(m_device, "Transparent.frag.spv");
		RHIPipelineVertexShaderStage vert_shader_stage_info;
		vert_shader_stage_info.shader_module = vertex_shader_module.getHandle();
		RHIPipelineFragmentShaderStage frag_shader_stage_info;
		frag_shader_stage_info.shader_module = fragment_shader_module.getHandle();

		// vertex input
		RHIPipelineVertexInputState vertex_input_state{};
		vertex_input_state.binding_description = Vertex::getBindingDescription();
		vertex_input_state.attribute_descriptions = Vertex::getAttributeDescriptions();

		// input assembly
		RHIPipelineInputAssemblyState input_assembly_state{};

		// viewport and scissors
		RHIPipelineViewportState viewport_state{};

		// rasterizer
		RHIPipelineRasterizationState rasterization_state{};

		// multisampling
		RHIPipelineMultisampleState multi_sample_state{};

		// depth stencil
		RHIPipelineDepthStencilState depth_stencil_state{};
		depth_stencil_state.depth_write_enable = false;

		// color blending
		RHIPipelineColorBlendState color_blending{};
		color_blending.blend_enable = true;

		// dynamic state
		RHIPipelineDynamicState dynamicState{};

		const std::vector<VkDescriptorSetLayout> descriptor_set_layouts = { m_ubo_descriptor_set_layout->getHandle() };
		const std::vector<VkPushConstantRange> push_constant_ranges = { { VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(SkyboxPushConstant) } };
		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = descriptor_set_layouts.size();
		pipeline_layout_info.pSetLayouts = descriptor_set_layouts.data();
		pipeline_layout_info.pushConstantRangeCount = push_constant_ranges.size();
		pipeline_layout_info.pPushConstantRanges = push_constant_ranges.data();
		m_pipeline_layout = std::make_unique<RHIPipelineLayout>(m_device, pipeline_layout_info);

		RHIPipelineState state;
		state.vertex_shader_stage = vert_shader_stage_info;
		state.fragment_shader_stage = frag_shader_stage_info;
		state.pipeline_layout = m_pipeline_layout->getHandle();
		state.render_pass = m_render_pass;
		state.vertex_input_state = vertex_input_state;
		state.input_assembly_state = input_assembly_state;
		state.viewport_state = viewport_state;
		state.rasterization_state = rasterization_state;
		state.multisample_state = multi_sample_state;
		state.depth_stencil_state = depth_stencil_state;
		state.color_blend_state = color_blending;
		state.dynamic_state = dynamicState;
		state.subpass_index = 0;
		m_pipeline = std::make_unique<RHIPipeline>(m_device);
		m_pipeline->createPipeline(state);
	}
}