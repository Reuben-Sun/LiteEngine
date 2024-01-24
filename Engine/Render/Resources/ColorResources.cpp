#include "ColorResources.h"

namespace ToolEngine
{
	ColorResources::ColorResources(RHIDevice& device, uint32_t width, uint32_t height, VkFormat format): m_device(device)
	{
		VkExtent2D extent = { width, height };
		m_image = std::make_unique<RHIImage>(m_device, extent, format,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	}
	ColorResources::~ColorResources()
	{
	}
}