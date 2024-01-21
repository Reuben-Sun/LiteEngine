#include "RHI/Public/RHITextureImage.h"
#include "Core/Path/Path.h"
#include "stb_image.h"
#include "RHI/Public/RHISingleTimeCommandBuffer.h"

namespace ToolEngine
{
	RHITextureImage::RHITextureImage(RHIDevice& device, const std::string& path): m_device(device)
	{
		std::string texture_local_path = Path::getInstance().getCurrentPath() + "/Assets/" + path;
		int texture_width, texture_height, texture_channels;
		stbi_uc* pixels = stbi_load(texture_local_path.c_str(), &texture_width, &texture_height, &texture_channels, STBI_rgb_alpha);
		if (!pixels)
		{
			LOG_ERROR("failed to load texture image!");
		}
		VkDeviceSize image_buffer_size = texture_width * texture_height * 4;
		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_memory;
		m_device.createBuffer(image_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_memory);
		void* data;
		vkMapMemory(m_device.getLogicalDevice(), staging_buffer_memory, 0, image_buffer_size, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(image_buffer_size));
		vkUnmapMemory(m_device.getLogicalDevice(), staging_buffer_memory);
		stbi_image_free(pixels);
		VkExtent2D extent{ texture_width, texture_height };
		m_image = std::make_unique<RHIImage>(m_device, extent, VK_FORMAT_R8G8B8A8_SRGB, 
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
			VK_IMAGE_ASPECT_COLOR_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		m_image->transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		createImageFormBuffer(staging_buffer, texture_width, texture_height);
		m_image->transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		vkDestroyBuffer(m_device.getLogicalDevice(), staging_buffer, nullptr);
		vkFreeMemory(m_device.getLogicalDevice(), staging_buffer_memory, nullptr);
		m_sampler = std::make_unique<RHISampler>(m_device);

		m_descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_descriptor.imageView = m_image->getImageView();
		m_descriptor.sampler = m_sampler->getHandle();
		LOG_INFO("Create texture image!");
	}
	RHITextureImage::~RHITextureImage()
	{
	}
	void RHITextureImage::createImageFormBuffer(VkBuffer buffer, uint32_t width, uint32_t height)
	{
		std::unique_ptr<RHISingleTimeCommandBuffer> command_buffer = std::make_unique<RHISingleTimeCommandBuffer>(m_device);
		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { width, height, 1 };
		command_buffer->copyImage(buffer, m_image->getHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	}
}