#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHIImage.h"
#include "RHI/Public/RHISampler.h"

namespace ToolEngine
{
	class ColorResources
	{
	public:
		ColorResources(RHIDevice& device, uint32_t width, uint32_t height, VkFormat format);
		~ColorResources();

		VkImageView getImageView() { return m_image->getImageView(); }
		VkSampler getSampler() { return m_sampler->getHandle(); }
		VkDescriptorImageInfo m_descriptor;
	protected:
		std::unique_ptr<RHIImage> m_image;
		std::unique_ptr<RHISampler> m_sampler;
		RHIDevice& m_device;
	};
}