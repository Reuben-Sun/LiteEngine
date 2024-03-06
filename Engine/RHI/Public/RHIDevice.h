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

		VkResult waitIdle() const { return vkDeviceWaitIdle(m_logical_device); }
		VkPhysicalDevice getPhysicalDevice() const { return m_physical_device; }
		VkDevice getLogicalDevice() const { return m_logical_device; }
		VkCommandPool getCommandPool() const { return m_command_pool->getHandle(); }
		VkFormat getDepthFormatDetail();
		VkQueue getGraphicsQueue() const { return m_graphics_queue; }
		uint32_t getGraphicsFamilyIndex() const { return m_graphics_family_index; }
		uint32_t findMemoryType(uint32_t type_filter, VkMemoryPropertyFlags properties);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& buffer_memory);
		void copyBuffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize size);

		void present(std::vector<VkSemaphore>& wait_semaphores, uint32_t image_index, std::vector<VkSwapchainKHR>& swapchains);
		void beginDebugUtilsLabel(VkCommandBuffer cmd, const VkDebugUtilsLabelEXT& label);
		void endDebugUtilsLabel(VkCommandBuffer cmd);

		const std::vector<const char*> m_device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	private:
		RHIInstance& m_instance;
		std::unique_ptr<RHICommandPool> m_command_pool;
		std::string m_name;
		VkPhysicalDevice m_physical_device{ VK_NULL_HANDLE };
		VkDevice m_logical_device{ VK_NULL_HANDLE };
		
		uint32_t m_graphics_family_index{ UINT32_MAX };
		VkQueue m_graphics_queue{ VK_NULL_HANDLE };
		VkQueue m_present_queue{ VK_NULL_HANDLE };

		std::vector<const char*> m_supported_device_extensions;

		void createPhysicalDevice();
		void createLogicalDevice();
		void loadExtensionFunctions();

		bool checkDeviceSupport(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		bool checkPresentSupport(VkPhysicalDevice device, uint32_t queue_family_index);
	};
}