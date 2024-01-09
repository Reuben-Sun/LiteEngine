#include "RHI/Public/RHIPipeline.h"

namespace ToolEngine
{
	RHIPipeline::RHIPipeline(RHIDevice& device): m_device(device)
	{
	}
	RHIPipeline::~RHIPipeline()
	{
		if (m_pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(m_device.getLogicalDevice(), m_pipeline, nullptr);
		}
	}
	void RHIPipeline::createPipeline(RHIPipelineState& pipeline_state)
	{
		m_pipeline_state = pipeline_state;

        std::vector<VkPipelineShaderStageCreateInfo> shader_stages = { m_pipeline_state.m_vertex_shader_stage, m_pipeline_state.m_fragment_shader_stage };

        VkGraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.stageCount = shader_stages.size();
        pipeline_info.pStages = shader_stages.data();
        pipeline_info.pVertexInputState = &m_pipeline_state.m_vertex_input_state;
        pipeline_info.pInputAssemblyState = &m_pipeline_state.m_input_assembly_state;
        pipeline_info.pViewportState = &m_pipeline_state.m_viewport_state;
        pipeline_info.pRasterizationState = &m_pipeline_state.m_rasterization_state;
        pipeline_info.pMultisampleState = &m_pipeline_state.m_multisample_state;
        pipeline_info.pDepthStencilState = &m_pipeline_state.m_depth_stencil_state;
        pipeline_info.pColorBlendState = &m_pipeline_state.m_color_blend_state;
        pipeline_info.pDynamicState = &m_pipeline_state.m_dynamic_state;
        pipeline_info.layout = m_pipeline_state.m_pipeline_layout;
        pipeline_info.renderPass = m_pipeline_state.m_render_pass;
        pipeline_info.subpass = m_pipeline_state.m_subpass_index;
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(m_device.getLogicalDevice(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_pipeline) != VK_SUCCESS)
        {
            LOG_ERROR("failed to create graphics pipeline!");
        }
        LOG_INFO("Create Graphics Pipeline!");
	}
}