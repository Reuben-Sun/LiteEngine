#include "DepthResources.h"

namespace ToolEngine
{
	DepthResources::DepthResources(RHIDevice& device, uint32_t width, uint32_t height) : m_device(device)
	{
		VkExtent2D extent = { width, height };
		m_image = std::make_unique<RHIImage>(m_device, extent, m_device.getDepthFormatDetail(),
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		m_image->transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_ASPECT_DEPTH_BIT);
	}
	DepthResources::~DepthResources()
	{
	}
}