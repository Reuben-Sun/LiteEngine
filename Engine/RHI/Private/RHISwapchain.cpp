#include "RHI/Public/RHISwapchain.h"

namespace ToolEngine
{
	RHISwapchain::RHISwapchain(RHIInstance& instance, RHIDevice& device, uint32_t width, uint32_t height)
		: m_instance(instance), m_device(device), m_width(width), m_height(height)
	{
		m_swapchain_support_details = SwapChainSupportDetails::getSwapChainSupportDetails(m_device.getPhysicalDevice(), m_instance.getSurfaceHandle());
		m_surface_format = chooseSwapSurfaceFormat();
		VkPresentModeKHR present_mode = chooseSwapPresentMode();
		VkExtent2D swap_extent = getSwapCurrentExtent();
		uint32_t image_count = getSwapchainImageCount();

		VkSwapchainCreateInfoKHR swapchain_create_info{};
		swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchain_create_info.surface = m_instance.getSurfaceHandle();
		swapchain_create_info.minImageCount = image_count;
		swapchain_create_info.imageFormat = m_surface_format.format;
		swapchain_create_info.imageColorSpace = m_surface_format.colorSpace;
		swapchain_create_info.imageExtent = swap_extent;
		swapchain_create_info.imageArrayLayers = 1;
		swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = QueueFamilyIndices::getQueueFamilyIndices(m_device.getPhysicalDevice(), m_instance.getSurfaceHandle());
		uint32_t queueFamilyIndices[] = { indices.graphics_family.value(), indices.present_family.value() };
		if (indices.graphics_family != indices.present_family)
		{
			swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchain_create_info.queueFamilyIndexCount = 2;
			swapchain_create_info.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		swapchain_create_info.preTransform = m_swapchain_support_details.capabilities.currentTransform;
		swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchain_create_info.presentMode = present_mode;
		swapchain_create_info.clipped = VK_TRUE;
		swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(device.getLogicalDevice(), &swapchain_create_info, nullptr, &m_swapchain) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create swap chain!");
		}
		LOG_INFO("Create Swapchain!");

		vkGetSwapchainImagesKHR(device.getLogicalDevice(), m_swapchain, &image_count, nullptr);
		m_swap_chain_images.resize(image_count);
		vkGetSwapchainImagesKHR(device.getLogicalDevice(), m_swapchain, &image_count, m_swap_chain_images.data());
		m_swap_chain_image_views.resize(image_count);
		for (uint32_t i = 0; i < image_count; i++)
		{
			VkImageViewCreateInfo image_view_create_info{};
			image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			image_view_create_info.image = m_swap_chain_images[i];
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = m_surface_format.format;
			image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.baseMipLevel = 0;
			image_view_create_info.subresourceRange.levelCount = 1;
			image_view_create_info.subresourceRange.baseArrayLayer = 0;
			image_view_create_info.subresourceRange.layerCount = 1;
			if (vkCreateImageView(m_device.getLogicalDevice(), &image_view_create_info, nullptr, &m_swap_chain_image_views[i]) != VK_SUCCESS)
			{
				LOG_ERROR("failed to create image views!");
			}
		}
		LOG_INFO("Create Swapchain Images!");
	}
	RHISwapchain::~RHISwapchain()
	{
		if (m_swapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(m_device.getLogicalDevice(), m_swapchain, nullptr);
		}
	}

	VkResult RHISwapchain::acquireNextTexture(Semaphore& semaphore, uint32_t& image_index)
	{
		VkResult result = vkAcquireNextImageKHR(m_device.getLogicalDevice(), m_swapchain, UINT64_MAX,
			semaphore.getHandle(), VK_NULL_HANDLE, &image_index);
		return result;
	}

	VkSurfaceFormatKHR RHISwapchain::chooseSwapSurfaceFormat()
	{
		for (const auto& available_format : m_swapchain_support_details.formats)
		{
			if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return available_format;
			}
		}
		return m_swapchain_support_details.formats[0];
	}

	VkPresentModeKHR RHISwapchain::chooseSwapPresentMode()
	{
		for (const auto& available_present_mode : m_swapchain_support_details.presentModes)
		{
			if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return available_present_mode;    // triple buffering
			}
		}
		return VK_PRESENT_MODE_FIFO_KHR;    // swap chain is a FIFO queue
	}

	VkExtent2D RHISwapchain::getSwapCurrentExtent()
	{
		m_width = m_swapchain_support_details.capabilities.currentExtent.width;
		m_height = m_swapchain_support_details.capabilities.currentExtent.height;
		return m_swapchain_support_details.capabilities.currentExtent;
	}

	uint32_t RHISwapchain::getSwapchainImageCount()
	{
		uint32_t image_count = m_swapchain_support_details.capabilities.minImageCount + 1;
		if (m_swapchain_support_details.capabilities.maxImageCount > 0 && image_count > m_swapchain_support_details.capabilities.maxImageCount)
		{
			image_count = m_swapchain_support_details.capabilities.maxImageCount;
		}
		return image_count;
	}
}