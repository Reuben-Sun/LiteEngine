#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIImage.h"

namespace ToolEngine
{
	class DepthResources
	{
	public:
		DepthResources(RHIDevice& device, VkExtent2D extent);
		virtual ~DepthResources();

	protected:
		std::unique_ptr<RHIImage> m_image;
		RHIDevice& m_device;
	};

	inline DepthResources::DepthResources(RHIDevice& device, VkExtent2D extent) : m_device(device)
	{
		m_image = std::make_unique<RHIImage>(m_device, extent, m_device.getDepthFormatDetail(),
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
			VK_IMAGE_ASPECT_DEPTH_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		m_image->transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}
	inline DepthResources::~DepthResources()
	{
	}
}