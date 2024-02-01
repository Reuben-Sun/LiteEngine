#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIPipeline.h"
#include "RHI/Public/RHIPipelineLayout.h"
#include "RHI/Public/RHIDescriptorSet.h"

namespace ToolEngine
{
	class GizmosPipeline final
	{
	public:
		GizmosPipeline(RHIDevice& device, VkRenderPass render_pass);
		~GizmosPipeline();

		RHIDescriptorSetLayout& getDescriptorSetLayout() const { return *m_ubo_descriptor_set_layout; }

		VkPipeline getHandle() const { return m_pipeline->getHandle(); }
		VkPipelineLayout getLayout() const { return m_pipeline_layout->getHandle(); }

	private:
		RHIDevice& m_device;
		VkRenderPass m_render_pass;

		std::unique_ptr<RHIPipelineLayout> m_pipeline_layout;
		std::unique_ptr<RHIPipeline> m_pipeline;
		std::unique_ptr<RHIDescriptorSetLayout> m_ubo_descriptor_set_layout;

		void createPipeline();
	};
}