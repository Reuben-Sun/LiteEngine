#include "RHI/Public/RHITextureImage.h"
#include "Core/Path/Path.h"
#include "stb_image.h"
#include "RHI/Public/RHISingleTimeCommandBuffer.h"

namespace ToolEngine
{
	RHITextureImage::RHITextureImage(RHIDevice& device, const std::string& path): m_device(device)
	{
		int texture_width, texture_height, texture_channels;
		stbi_uc* pixels = stbi_load(path.c_str(), &texture_width, &texture_height, &texture_channels, STBI_rgb_alpha);
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
		LOG_INFO("Create texture image {}!", path);
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


	RHITextureCube::RHITextureCube(RHIDevice& device, const std::vector<std::string>& paths): m_device(device)
	{
		std::vector<stbi_uc*> pixels;
		std::vector<int> texture_widths;
		std::vector<int> texture_heights;
		std::vector<int> texture_channels;
		for (const auto& path : paths)
		{
			int texture_width, texture_height, texture_channel;
			stbi_uc* pixel = stbi_load(path.c_str(), &texture_width, &texture_height, &texture_channel, STBI_rgb_alpha);
			if (!pixel)
			{
				LOG_ERROR("failed to load texture image!");
			}
			pixels.push_back(pixel);
			texture_widths.push_back(texture_width);
			texture_heights.push_back(texture_height);
			texture_channels.push_back(texture_channel);
		}
		int each_pixel_size = texture_widths[0] * texture_heights[0] * 4;
		unsigned char* all_pixels = new unsigned char[each_pixel_size * 6];
		for (int i = 0; i < 6; i++)
		{
			memcpy(all_pixels + each_pixel_size * i, pixels[i], each_pixel_size);
		}
		VkDeviceSize image_buffer_size = texture_widths[0] * texture_heights[0] * 4 * 6;
		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_memory;
		m_device.createBuffer(image_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_memory);
		void* data;
		vkMapMemory(m_device.getLogicalDevice(), staging_buffer_memory, 0, image_buffer_size, 0, &data);
		memcpy(data, all_pixels, static_cast<size_t>(image_buffer_size));
		vkUnmapMemory(m_device.getLogicalDevice(), staging_buffer_memory);
		for (int i = 0; i < pixels.size(); i++)
		{
			stbi_image_free(pixels[i]);
		}
		delete[] all_pixels;
		VkExtent2D extent{ texture_widths[0], texture_heights[0] };
		m_image = std::make_unique<RHIImage>(m_device, extent, VK_FORMAT_R8G8B8A8_SRGB,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ImageType::TEXTURE_CUBE);
		m_image->transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		createCubeImageFormBuffer(staging_buffer, texture_widths[0], texture_heights[0]);
		m_image->transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		vkDestroyBuffer(m_device.getLogicalDevice(), staging_buffer, nullptr);
		vkFreeMemory(m_device.getLogicalDevice(), staging_buffer_memory, nullptr);
		m_sampler = std::make_unique<RHISampler>(m_device);

		m_descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_descriptor.imageView = m_image->getImageView();
		m_descriptor.sampler = m_sampler->getHandle();
		LOG_INFO("Create cube texture image!");
	}
	RHITextureCube::RHITextureCube(RHIDevice& device, const std::string& ktx_path): m_device(device)
	{
		ktxTexture* texture;
		ktxResult result = ktxTexture_CreateFromNamedFile(ktx_path.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &texture);
		int width = texture->baseWidth;
		int height = texture->baseHeight;
		int mipmap_count = texture->numLevels;
		ktx_uint8_t* ktxTextureData = ktxTexture_GetData(texture);
		ktx_size_t ktxTextureSize = texture->dataSize;

		VkDeviceSize image_buffer_size = ktxTextureSize;
		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_memory;
		m_device.createBuffer(image_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_memory);
		void* data;
		vkMapMemory(m_device.getLogicalDevice(), staging_buffer_memory, 0, image_buffer_size, 0, &data);
		memcpy(data, ktxTextureData, static_cast<size_t>(image_buffer_size));
		vkUnmapMemory(m_device.getLogicalDevice(), staging_buffer_memory);

		VkExtent2D extent{ width, height };
		m_image = std::make_unique<RHIImage>(m_device, extent, VK_FORMAT_R8G8B8A8_SRGB,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, ImageType::TEXTURE_CUBE, mipmap_count);
		m_image->transitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		createCubeImageFormKtxAndBuffer(staging_buffer, width, height, texture);
		m_image->transitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		vkDestroyBuffer(m_device.getLogicalDevice(), staging_buffer, nullptr);
		vkFreeMemory(m_device.getLogicalDevice(), staging_buffer_memory, nullptr);

		m_sampler = std::make_unique<RHISampler>(m_device);

		m_descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_descriptor.imageView = m_image->getImageView();
		m_descriptor.sampler = m_sampler->getHandle();
		LOG_INFO("Create cube texture image!");
	}
	RHITextureCube::~RHITextureCube()
	{
	}
	void RHITextureCube::createCubeImageFormBuffer(VkBuffer buffer, uint32_t width, uint32_t height)
	{
		std::unique_ptr<RHISingleTimeCommandBuffer> command_buffer = std::make_unique<RHISingleTimeCommandBuffer>(m_device);
		std::vector<VkBufferImageCopy> regions;
		for (int face = 0; face < 6; face++)
		{
			VkBufferImageCopy region{};
			region.bufferOffset = width * height * 4 * face;
			region.bufferRowLength = width;
			region.bufferImageHeight = height;
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = face;
			region.imageSubresource.layerCount = 1;
			region.imageExtent = { width, height, 1 };
			regions.push_back(region);
		}
		
		command_buffer->copyImage(buffer, m_image->getHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, regions.size(), regions.data());
	}
	void RHITextureCube::createCubeImageFormKtxAndBuffer(VkBuffer buffer, uint32_t width, uint32_t height, ktxTexture* texture)
	{
		std::unique_ptr<RHISingleTimeCommandBuffer> command_buffer = std::make_unique<RHISingleTimeCommandBuffer>(m_device);
		std::vector<VkBufferImageCopy> regions;
		for (int face = 0; face < 6; face++)
		{
			for (int mip_level = 0; mip_level < texture->numLevels; mip_level++)
			{
				ktx_size_t offset;
				KTX_error_code result = ktxTexture_GetImageOffset(texture, mip_level, 0, face, &offset);

				VkBufferImageCopy region{};
				region.bufferOffset = offset;
				region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				region.imageSubresource.mipLevel = mip_level;
				region.imageSubresource.baseArrayLayer = face;
				region.imageSubresource.layerCount = 1;
				region.imageExtent = { width >> mip_level, height >> mip_level, 1 };
				regions.push_back(region);
			}
		}

		command_buffer->copyImage(buffer, m_image->getHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, regions.size(), regions.data());
	}
}