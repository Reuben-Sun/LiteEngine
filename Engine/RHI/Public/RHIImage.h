#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHIImage
	{
	public:
		RHIImage(RHIDevice& device, VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
		~RHIImage();

		void transitionImageLayout(VkImageLayout new_layout);
		VkImage getHandle() const { return m_image; }
		VkFormat getFormat() const { return m_format; }
	private:
		RHIDevice& m_device;
		VkImage m_image{ VK_NULL_HANDLE };
		VkDeviceMemory m_image_memory{ VK_NULL_HANDLE };
		VkFormat m_format = VK_FORMAT_R8G8B8A8_SRGB;

		uint32_t findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties);
	};
}