#include "ColorResources.h"

namespace ToolEngine
{
	ColorResources::ColorResources(RHIDevice& device, uint32_t width, uint32_t height, VkFormat format): m_device(device)
	{
		VkExtent2D extent = { width, height };
		m_image = std::make_unique<RHIImage>(m_device, extent, format,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		m_sampler = std::make_unique<RHISampler>(m_device);
		m_descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_descriptor.imageView = m_image->getImageView();
		m_descriptor.sampler = m_sampler->getHandle();
	}
	ColorResources::~ColorResources()
	{
	}
}