#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHIRenderPass
	{
	public:
		RHIRenderPass(RHIDevice& device);
		virtual ~RHIRenderPass();

		VkRenderPass getHandle() const { return m_render_pass; }
	protected:
		VkRenderPass m_render_pass{ VK_NULL_HANDLE };
		RHIDevice& m_device;
	};

	inline RHIRenderPass::RHIRenderPass(RHIDevice& device): m_device(device)
	{
	}
	inline RHIRenderPass::~RHIRenderPass()
	{
		if (m_render_pass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(m_device.getLogicalDevice(), m_render_pass, nullptr);
		}
	}
}