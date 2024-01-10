#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHIRenderPass
	{
	public:
		RHIRenderPass(RHIDevice& device, VkFormat format);
		virtual ~RHIRenderPass();

		VkRenderPass getHandle() const { return m_render_pass; }
	protected:
		VkRenderPass m_render_pass;
		RHIDevice& m_device;
		VkFormat m_format;
	};

	RHIRenderPass::RHIRenderPass(RHIDevice& device, VkFormat format): m_device(device), m_format(format)
	{
	}
	RHIRenderPass::~RHIRenderPass()
	{
		if (m_render_pass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(m_device.getLogicalDevice(), m_render_pass, nullptr);
		}
	}
}