#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIImage.h"

namespace ToolEngine
{
	class ColorResources
	{
	public:
		ColorResources(RHIDevice& device, uint32_t width, uint32_t height, VkFormat format);
		~ColorResources();

		VkImageView getImageView() { return m_image->getImageView(); }
	protected:
		std::unique_ptr<RHIImage> m_image;
		RHIDevice& m_device;
	};
}