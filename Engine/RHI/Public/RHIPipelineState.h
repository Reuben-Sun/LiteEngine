#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	struct RHIPipelineState
	{
		VkPipelineShaderStageCreateInfo m_vertex_shader_stage{};
		VkPipelineShaderStageCreateInfo m_fragment_shader_stage{};
		VkPipelineLayout m_pipeline_layout{};	
		VkRenderPass m_render_pass{};	
		// TODO: constant state
		VkPipelineVertexInputStateCreateInfo m_vertex_input_state{};	 
		VkPipelineInputAssemblyStateCreateInfo m_input_assembly_state{};	
		// TODO: tessellation state
		VkPipelineViewportStateCreateInfo m_viewport_state{};	
		VkPipelineRasterizationStateCreateInfo m_rasterization_state{};	
		VkPipelineMultisampleStateCreateInfo m_multisample_state{};	
		VkPipelineDepthStencilStateCreateInfo m_depth_stencil_state{};	
		VkPipelineColorBlendStateCreateInfo m_color_blend_state{};	
		VkPipelineDynamicStateCreateInfo m_dynamic_state{};	
		uint32_t m_subpass_index{ 0 };	
		void reset();
	};

	void RHIPipelineState::reset()
	{
		m_vertex_shader_stage = {};
		m_fragment_shader_stage = {};
		m_pipeline_layout = nullptr;
		m_render_pass = nullptr;
		m_vertex_input_state = {};
		m_input_assembly_state = {};
		m_viewport_state = {};
		m_rasterization_state = {};
		m_multisample_state = {};
		m_color_blend_state = {};
		m_dynamic_state = {};
		m_subpass_index = 0;
	}
}