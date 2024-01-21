#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHISampler.h"
#include "RHI/Public/RHIImage.h"

namespace ToolEngine
{
	class TextureImage
	{
	public:
		TextureImage(RHIDevice& device, const std::string& path);
		~TextureImage();


	private:
		std::unique_ptr<RHIImage> m_image;
		std::unique_ptr<RHISampler> m_sampler;
		RHIDevice& m_device;
		VkDescriptorImageInfo m_descriptor_image_info;
		void createImageFormBuffer(VkBuffer buffer, uint32_t width, uint32_t height);
	};
}