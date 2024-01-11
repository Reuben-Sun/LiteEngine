#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIInstance.h"
#include "RHICommandPool.h"

namespace ToolEngine
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphics_family;
		std::optional<uint32_t> present_family;

		bool isComplete()
		{
			return graphics_family.has_value() && present_family.has_value();
		}

		static QueueFamilyIndices getQueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface);
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		static SwapChainSupportDetails getSwapChainSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
	};

	class RHIDevice
	{
	public:
		RHIDevice(RHIInstance& instance);
		~RHIDevice();

		VkPhysicalDevice getPhysicalDevice() const { return m_physical_device; }
		VkDevice getLogicalDevice() const { return m_logical_device; }
		VkCommandPool getCommandPool() const { return m_command_pool->getHandle(); }
		VkFormat getDepthFormatDetail();
		VkQueue getGraphicsQueue() const { return m_graphics_queue; }

		void present(std::vector<VkSemaphore>& wait_semaphores, uint32_t image_index, std::vector<VkSwapchainKHR>& swapchains);

		const std::vector<const char*> m_device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	private:
		RHIInstance& m_instance;
		std::unique_ptr<RHICommandPool> m_command_pool;
		std::string m_name;
		VkPhysicalDevice m_physical_device{ VK_NULL_HANDLE };
		VkDevice m_logical_device{ VK_NULL_HANDLE };

		VkQueue m_graphics_queue{ VK_NULL_HANDLE };
		VkQueue m_present_queue{ VK_NULL_HANDLE };

		std::vector<const char*> m_supported_device_extensions;

		void createPhysicalDevice();
		void createLogicalDevice();

		bool checkDeviceSupport(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		bool checkPresentSupport(VkPhysicalDevice device, uint32_t queue_family_index);
	};
}