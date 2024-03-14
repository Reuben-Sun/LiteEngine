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

        std::vector<VkPipelineShaderStageCreateInfo> shader_stages = { 
            m_pipeline_state.vertex_shader_stage.getHandle(), 
            m_pipeline_state.fragment_shader_stage.getHandle()
        };
        VkPipelineVertexInputStateCreateInfo vertex_input_state = m_pipeline_state.vertex_input_state.getHandle();
        VkPipelineInputAssemblyStateCreateInfo input_assembly_state = m_pipeline_state.input_assembly_state.getHandle();
        VkPipelineViewportStateCreateInfo viewport_state = m_pipeline_state.viewport_state.getHandle();
        VkPipelineRasterizationStateCreateInfo rasterization_state = m_pipeline_state.rasterization_state.getHandle();
        VkPipelineMultisampleStateCreateInfo multisample_state = m_pipeline_state.multisample_state.getHandle();
        VkPipelineDepthStencilStateCreateInfo depth_stencil_state = m_pipeline_state.depth_stencil_state.getHandle();
        VkPipelineColorBlendStateCreateInfo color_blend_state = m_pipeline_state.color_blend_state.getHandle();
        VkPipelineDynamicStateCreateInfo dynamic_state = m_pipeline_state.dynamic_state.getHandle();

        VkGraphicsPipelineCreateInfo pipeline_info{};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_info.stageCount = shader_stages.size();
        pipeline_info.pStages = shader_stages.data();
        pipeline_info.pVertexInputState = &vertex_input_state;
        pipeline_info.pInputAssemblyState = &input_assembly_state;
        pipeline_info.pViewportState = &viewport_state;
        pipeline_info.pRasterizationState = &rasterization_state;
        pipeline_info.pMultisampleState = &multisample_state;
        pipeline_info.pDepthStencilState = &depth_stencil_state;
        pipeline_info.pColorBlendState = &color_blend_state;
        pipeline_info.pDynamicState = &dynamic_state;
        pipeline_info.layout = m_pipeline_state.pipeline_layout;
        pipeline_info.renderPass = m_pipeline_state.render_pass;
        pipeline_info.subpass = m_pipeline_state.subpass_index;
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(m_device.getLogicalDevice(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_pipeline) != VK_SUCCESS)
        {
            LOG_ERROR("failed to create graphics pipeline!");
        }
        LOG_INFO("Create Graphics Pipeline!");
	}
}