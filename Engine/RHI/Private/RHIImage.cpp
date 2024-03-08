#include "RHI/Public/RHIImage.h"
#include "RHI/Public/RHISingleTimeCommandBuffer.h"

namespace ToolEngine
{
	RHIImage::RHIImage(RHIDevice& device, VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageAspectFlags aspect_flags, VkMemoryPropertyFlags properties, ImageType type)
		: m_device(device), m_format(format), m_type(type)
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
		if (m_type == ImageType::TEXTURE_CUBE)
		{
			image_create_info.arrayLayers = 6;
			image_create_info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		}

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

		VkImageViewCreateInfo image_view_create_info{};
		image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		image_view_create_info.image = m_image;
		image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.format = m_format;
		image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.subresourceRange.aspectMask = aspect_flags;
		image_view_create_info.subresourceRange.baseMipLevel = 0;
		image_view_create_info.subresourceRange.levelCount = 1;
		image_view_create_info.subresourceRange.baseArrayLayer = 0;
		image_view_create_info.subresourceRange.layerCount = 1;
		if (m_type == ImageType::TEXTURE_CUBE)
		{
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			image_view_create_info.subresourceRange.layerCount = 6;
		}
		if (vkCreateImageView(m_device.getLogicalDevice(), &image_view_create_info, nullptr, &m_image_view) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create image views!");
		}
		LOG_INFO("Create image view!");
	}
	RHIImage::~RHIImage()
	{
		vkDestroyImage(m_device.getLogicalDevice(), m_image, nullptr);
		vkFreeMemory(m_device.getLogicalDevice(), m_image_memory, nullptr);
		if (m_image_view != VK_NULL_HANDLE)
		{
			vkDestroyImageView(m_device.getLogicalDevice(), m_image_view, nullptr);
		}
	}
	void RHIImage::transitionImageLayout(VkImageLayout old_layout, VkImageLayout new_layout, VkImageAspectFlags aspect_flags)
	{
		std::unique_ptr<RHISingleTimeCommandBuffer> command_buffer = std::make_unique<RHISingleTimeCommandBuffer>(m_device);
		// use memory barrier to make sure image load before next operation, especially asynchronous programs
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = old_layout;
		barrier.newLayout = new_layout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = m_image;
		barrier.subresourceRange.aspectMask = aspect_flags;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		if (m_type == ImageType::TEXTURE_CUBE)
		{
			barrier.subresourceRange.layerCount = 6;
		}

		VkPipelineStageFlags source_stage;
		VkPipelineStageFlags destination_stage;
		if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			// when image is first used, transition data to image 
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			// after data is loaded, transition image for shader reading
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
		{
			// when image (used for depth/stencil buffer) is first used, transition data to image
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destination_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		}
		else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

			source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else
		{
			LOG_ERROR("unsupported layout transition!");
		}
		command_buffer->pipelineBarrier(source_stage, destination_stage, barrier);
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