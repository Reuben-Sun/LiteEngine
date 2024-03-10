#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	struct RHIPipelineVertexShaderStage
	{
		std::string entry_name = "MainVS";
		VkShaderModule shader_module;

		VkPipelineShaderStageCreateInfo getHandle()
		{
			VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
			vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vert_shader_stage_info.module = shader_module;
			vert_shader_stage_info.pName = entry_name.c_str();
			return vert_shader_stage_info;
		}
	};

	struct RHIPipelineFragmentShaderStage
	{
		std::string entry_name = "MainPS";
		VkShaderModule shader_module;

		VkPipelineShaderStageCreateInfo getHandle()
		{
			VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
			frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			frag_shader_stage_info.module = shader_module;
			frag_shader_stage_info.pName = entry_name.c_str();
			return frag_shader_stage_info;
		}
	};

	struct RHIPipelineVertexInputState
	{
		VkPipelineVertexInputStateCreateInfo getHandle()
		{

		}
	};

	struct RHIPipelineInputAssemblyState
	{
		VkPipelineInputAssemblyStateCreateInfo getHandle()
		{

		}
	};

	struct RHIPipelineViewportState
	{
		VkPipelineViewportStateCreateInfo getHandle()
		{

		}
	};

	struct RHIPipelineRasterizationState
	{
		VkPipelineRasterizationStateCreateInfo getHandle()
		{

		}
	};

	struct RHIPipelineMultisampleState
	{
		VkPipelineMultisampleStateCreateInfo getHandle()
		{

		}
	};

	struct RHIPipelineDepthStencilState
	{
		VkPipelineDepthStencilStateCreateInfo getHandle()
		{

		}
	};

	struct RHIPipelineColorBlendState
	{
		VkPipelineColorBlendStateCreateInfo getHandle()
		{

		}
	};

	struct RHIPipelineDynamicState
	{
		VkPipelineDynamicStateCreateInfo getHandle()
		{

		}
	};

	struct RHIPipelineState
	{
		RHIPipelineVertexShaderStage m_vertex_shader_stage{};
		RHIPipelineFragmentShaderStage m_fragment_shader_stage{};
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
	};
}