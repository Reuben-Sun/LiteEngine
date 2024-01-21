#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHISampler.h"
#include "RHI/Public/RHIImage.h"

namespace ToolEngine
{
	class RHITextureImage
	{
	public:
		RHITextureImage(RHIDevice& device, const std::string& path);
		~RHITextureImage();
		VkDescriptorImageInfo m_descriptor;
	private:
		std::unique_ptr<RHIImage> m_image;
		std::unique_ptr<RHISampler> m_sampler;
		RHIDevice& m_device;
		
		void createImageFormBuffer(VkBuffer buffer, uint32_t width, uint32_t height);
	};
}