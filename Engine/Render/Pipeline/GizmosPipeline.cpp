#include "GizmosPipeline.h"
#include "RHI/Public/RHIShader.h"
#include "Geometry/Vertex.h"

namespace ToolEngine
{
	GizmosPipeline::GizmosPipeline(RHIDevice& device, VkRenderPass render_pass)
		: m_device(device), m_render_pass(render_pass)
	{
		std::vector<RHIDescriptorType> layout_descriptor;
		layout_descriptor.push_back(RHIDescriptorType::ConstantBuffer);
		m_ubo_descriptor_set_layout = std::make_unique<RHIDescriptorSetLayout>(m_device, layout_descriptor);
		createPipeline();
		LOG_INFO("Create GizmosPipeline!");
	}
	GizmosPipeline::~GizmosPipeline()
	{
	}
	void GizmosPipeline::createPipeline()
	{
		// shader
		RHIShader vertex_shader_module(m_device, "GizmosLine.vert.spv");
		RHIShader fragment_shader_module(m_device, "GizmosLine.frag.spv");
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
		rasterization_state.polygon_mode = VK_POLYGON_MODE_LINE;

		// multisampling
		RHIPipelineMultisampleState multi_sample_state{};

		// depth stencil
		RHIPipelineDepthStencilState depth_stencil_state{};
		depth_stencil_state.depth_write_enable = false;

		// color blending
		RHIPipelineColorBlendState color_blending{};

		// dynamic state
		RHIPipelineDynamicState dynamicState{};

		const std::vector<VkDescriptorSetLayout> descriptor_set_layouts = { m_ubo_descriptor_set_layout->getHandle() };
		const std::vector<VkPushConstantRange> push_constant_ranges = { { VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(GizmoPushConstant) } };
		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = descriptor_set_layouts.size();
		pipeline_layout_info.pSetLayouts = descriptor_set_layouts.data();
		pipeline_layout_info.pushConstantRangeCount = push_constant_ranges.size();
		pipeline_layout_info.pPushConstantRanges = push_constant_ranges.data();
		m_pipeline_layout = std::make_unique<RHIPipelineLayout>(m_device, pipeline_layout_info);

		RHIPipelineState m_state;
		m_state.m_vertex_shader_stage = vert_shader_stage_info;
		m_state.m_fragment_shader_stage = frag_shader_stage_info;
		m_state.m_pipeline_layout = m_pipeline_layout->getHandle();
		m_state.m_render_pass = m_render_pass;
		m_state.m_vertex_input_state = vertex_input_state;
		m_state.m_input_assembly_state = input_assembly_state;
		m_state.m_viewport_state = viewport_state;
		m_state.m_rasterization_state = rasterization_state;
		m_state.m_multisample_state = multi_sample_state;
		m_state.m_depth_stencil_state = depth_stencil_state;
		m_state.m_color_blend_state = color_blending;
		m_state.m_dynamic_state = dynamicState;
		m_state.m_subpass_index = 0;
		m_pipeline = std::make_unique<RHIPipeline>(m_device);
		m_pipeline->createPipeline(m_state);
	}
}