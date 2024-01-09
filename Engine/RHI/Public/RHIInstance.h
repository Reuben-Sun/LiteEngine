#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "Window/GlfwWindow.h"

namespace ToolEngine
{
	class RHIInstance
	{
	public:
		RHIInstance(GlfwWindow& window);
		~RHIInstance();
		VkInstance getHandle() const { return m_instance; }
		VkSurfaceKHR getSurfaceHandle() const { return m_surface; }

		bool isEnableValidationLayers() const { return m_enable_validation_layers; }
		const std::vector<const char*> m_validation_layers = { "VK_LAYER_KHRONOS_validation" };
	private:
		VkInstance m_instance{ VK_NULL_HANDLE };
		VkSurfaceKHR m_surface{ VK_NULL_HANDLE };
		VkDebugUtilsMessengerEXT debug_utils_messenger{ VK_NULL_HANDLE };
		bool m_enable_validation_layers = true;

		bool checkValidationLayerSupport();
	};
}