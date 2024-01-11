#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"
#include "RHIInstance.h"

namespace ToolEngine
{
	class RHISwapchain
	{
	public:
		RHISwapchain(RHIInstance& instance, RHIDevice& device, uint32_t width, uint32_t height);
		~RHISwapchain();

		VkSwapchainKHR getHandle() const { return m_swapchain; }
		uint32_t getImageCount() const { return static_cast<uint32_t>(m_swap_chain_images.size()); }
		VkImageView getImageView(uint32_t index) const { return m_swap_chain_image_views[index]; }
		VkFormat getFormat() const { return m_surface_format.format; }
		uint32_t getWidth() const { return m_width; }
		uint32_t getHeight() const { return m_height; }
		void present(VkSemaphore* wait_semaphores, uint32_t image_index);
	private:
		RHIInstance& m_instance;
		RHIDevice& m_device;
		uint32_t m_width;
		uint32_t m_height;
		VkSwapchainKHR m_swapchain{ VK_NULL_HANDLE };
		std::vector<VkImage> m_swap_chain_images;
		std::vector<VkImageView> m_swap_chain_image_views;
		VkSurfaceFormatKHR m_surface_format;

		SwapChainSupportDetails m_swapchain_support_details;
		VkExtent2D getSwapCurrentExtent();
		VkSurfaceFormatKHR chooseSwapSurfaceFormat();
		VkPresentModeKHR chooseSwapPresentMode();
		uint32_t getSwapchainImageCount();
	};
}