#include "RHI/Public/RHIImage.h"

namespace ToolEngine
{
	RHIImage::RHIImage(RHIDevice& device, VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
		: m_device(device), m_format(format)
	{
		VkImageCreateInfo image_create_info{};
		image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_create_info.imageType = VK_IMAGE_TYPE_2D;
		image_create_info.extent.width = extent.width;
		image_create_info.extent.height = extent.height;
		image_create_info.extent.depth = 1;
		image_create_info.mipLevels = 1;
		image_create_info.arrayLayers = 1;
		image_create_info.format = format;
		image_create_info.tiling = tiling;
		image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_create_info.usage = usage;
		image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(m_device.getLogicalDevice(), &image_create_info, nullptr, &m_image) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create image!");
		}

		VkMemoryRequirements mem_requirements;
		vkGetImageMemoryRequirements(m_device.getLogicalDevice(), m_image, &mem_requirements);

		VkMemoryAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize = mem_requirements.size;
		alloc_info.memoryTypeIndex = findMemoryType(mem_requirements.memoryTypeBits, properties);

		if (vkAllocateMemory(m_device.getLogicalDevice(), &alloc_info, nullptr, &m_image_memory) != VK_SUCCESS)
		{
			LOG_ERROR("failed to allocate image memory!");
		}

		vkBindImageMemory(m_device.getLogicalDevice(), m_image, m_image_memory, 0);
		LOG_INFO("Create Image!");
	}
	RHIImage::~RHIImage()
	{
		vkDestroyImage(m_device.getLogicalDevice(), m_image, nullptr);
		vkFreeMemory(m_device.getLogicalDevice(), m_image_memory, nullptr);
	}
	void RHIImage::transitionImageLayout(VkImageLayout new_layout)
	{
	}
	uint32_t RHIImage::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		VkPhysicalDeviceMemoryProperties mem_properties;
		vkGetPhysicalDeviceMemoryProperties(m_device.getPhysicalDevice(), &mem_properties);

		for (uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
		{
			if ((typeFilter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}
		LOG_ERROR("failed to find suitable memory type!");
		return 0;
	}
}