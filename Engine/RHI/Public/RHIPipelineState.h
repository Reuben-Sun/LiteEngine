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
		VkVertexInputBindingDescription binding_description;
		std::vector<VkVertexInputAttributeDescription> attribute_descriptions;
		VkPipelineVertexInputStateCreateInfo getHandle()
		{
			VkPipelineVertexInputStateCreateInfo vertex_input_state{};
			vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertex_input_state.vertexBindingDescriptionCount = 1;
			vertex_input_state.pVertexBindingDescriptions = &binding_description;
			vertex_input_state.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size());
			vertex_input_state.pVertexAttributeDescriptions = attribute_descriptions.data();
			return vertex_input_state;
		}
	};

	struct RHIPipelineInputAssemblyState
	{
		VkPipelineInputAssemblyStateCreateInfo getHandle()
		{
			VkPipelineInputAssemblyStateCreateInfo input_assembly_state{};
			input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			input_assembly_state.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			input_assembly_state.primitiveRestartEnable = VK_FALSE;
			return input_assembly_state;
		}
	};

	struct RHIPipelineViewportState
	{
		VkPipelineViewportStateCreateInfo getHandle()
		{
			VkPipelineViewportStateCreateInfo viewport_state{};
			viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewport_state.viewportCount = 1;
			viewport_state.scissorCount = 1;
			return viewport_state;
		}
	};

	struct RHIPipelineRasterizationState
	{
		VkPolygonMode polygon_mode = VK_POLYGON_MODE_FILL;
		VkCullModeFlags cull_mode = VK_CULL_MODE_NONE;
		VkPipelineRasterizationStateCreateInfo getHandle()
		{
			VkPipelineRasterizationStateCreateInfo rasterization_state{};
			rasterization_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterization_state.depthClampEnable = VK_FALSE;
			rasterization_state.rasterizerDiscardEnable = VK_FALSE;
			rasterization_state.polygonMode = polygon_mode;
			rasterization_state.lineWidth = 1.0f;
			rasterization_state.cullMode = cull_mode;
			rasterization_state.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
			rasterization_state.depthBiasEnable = VK_FALSE;
			return rasterization_state;
		}
	};

	struct RHIPipelineMultisampleState
	{
		bool enable = false;
		VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT;
		VkPipelineMultisampleStateCreateInfo getHandle()
		{
			VkPipelineMultisampleStateCreateInfo multi_sample_state{};
			multi_sample_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multi_sample_state.sampleShadingEnable = enable ? VK_TRUE : VK_FALSE;
			multi_sample_state.rasterizationSamples = sample_count;
			return multi_sample_state;
		}
	};

	struct RHIPipelineDepthStencilState
	{
		bool depth_test_enable = true;
		bool depth_write_enable = true;
		VkCompareOp depth_op_func = VK_COMPARE_OP_LESS;
		bool stencil_test_enable = false;
		VkPipelineDepthStencilStateCreateInfo getHandle()
		{
			VkPipelineDepthStencilStateCreateInfo depth_stencil_state{};
			depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			depth_stencil_state.depthTestEnable = depth_test_enable ? VK_TRUE : VK_FALSE;
			depth_stencil_state.depthWriteEnable = depth_write_enable ? VK_TRUE : VK_FALSE;
			depth_stencil_state.depthCompareOp = depth_op_func;
			depth_stencil_state.depthBoundsTestEnable = VK_FALSE;
			depth_stencil_state.minDepthBounds = 0.0f; // Optional
			depth_stencil_state.maxDepthBounds = 1.0f; // Optional
			depth_stencil_state.stencilTestEnable = VK_FALSE;
			depth_stencil_state.front = {}; // Optional
			depth_stencil_state.back = {}; // Optional
			return depth_stencil_state;
		}
	};

	struct RHIPipelineColorBlendState
	{
		bool blend_enable = false;
		VkPipelineColorBlendAttachmentState color_blend_attachment_state;
		// TODO: translucency render
		VkPipelineColorBlendStateCreateInfo getHandle()
		{
			color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			color_blend_attachment_state.blendEnable = blend_enable ? VK_TRUE : VK_FALSE;
			VkPipelineColorBlendStateCreateInfo color_blending{};
			color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			color_blending.logicOpEnable = VK_FALSE;
			color_blending.logicOp = VK_LOGIC_OP_COPY;
			color_blending.attachmentCount = 1;
			color_blending.pAttachments = &color_blend_attachment_state;
			color_blending.blendConstants[0] = 0.0f;
			color_blending.blendConstants[1] = 0.0f;
			color_blending.blendConstants[2] = 0.0f;
			color_blending.blendConstants[3] = 0.0f;
			return color_blending;
		}
	};

	struct RHIPipelineDynamicState
	{
		std::vector<VkDynamicState> dynamic_states = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
		VkPipelineDynamicStateCreateInfo getHandle()
		{
			VkPipelineDynamicStateCreateInfo dynamic_state{};
			dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
			dynamic_state.pDynamicStates = dynamic_states.data();
			return dynamic_state;
		}
	};

	struct RHIPipelineState
	{
		RHIPipelineVertexShaderStage vertex_shader_stage{};
		RHIPipelineFragmentShaderStage fragment_shader_stage{};
		VkPipelineLayout pipeline_layout{};	
		VkRenderPass render_pass{};	
		// TODO: constant state
		RHIPipelineVertexInputState vertex_input_state{};
		RHIPipelineInputAssemblyState input_assembly_state{};
		// TODO: tessellation state
		RHIPipelineViewportState viewport_state{};
		RHIPipelineRasterizationState rasterization_state{};
		RHIPipelineMultisampleState multisample_state{};
		RHIPipelineDepthStencilState depth_stencil_state{};
		RHIPipelineColorBlendState color_blend_state{};
		RHIPipelineDynamicState dynamic_state{};
		uint32_t subpass_index{ 0 };	
	};
}