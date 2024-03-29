#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	enum class ImageType
	{
		TEXTURE_2D,
		TEXTURE_CUBE
	};
	class RHIImage
	{
	public:
		RHIImage(RHIDevice& device, VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageAspectFlags aspect_flags, VkMemoryPropertyFlags properties, ImageType type = ImageType::TEXTURE_2D, uint32_t mipmap_count = 1);
		~RHIImage();

		void transitionImageLayout(VkImageLayout old_layout, VkImageLayout new_layout, VkImageAspectFlags aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT);
		VkImage getHandle() const { return m_image; }
		VkFormat getFormat() const { return m_format; }
		VkImageView getImageView() const { return m_image_view; }
	private:
		RHIDevice& m_device;
		VkImage m_image{ VK_NULL_HANDLE };
		VkImageView m_image_view{ VK_NULL_HANDLE };
		VkDeviceMemory m_image_memory{ VK_NULL_HANDLE };
		VkFormat m_format = VK_FORMAT_R8G8B8A8_SRGB;
		ImageType m_type = ImageType::TEXTURE_2D;
		uint32_t m_mipmap_count;

		uint32_t findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties);
	};
}