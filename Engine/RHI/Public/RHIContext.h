#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "Window/GlfwWindow.h"
#include "RHIInstance.h"
#include "RHIDevice.h"
#include "RHISwapchain.h"
#include "RHIDescriptorPool.h"

namespace ToolEngine
{
	class RHIContext
	{
	public:
		RHIContext(GlfwWindow& window);
		~RHIContext();

		void tick();

		std::unique_ptr<RHIInstance> m_instance;
		std::unique_ptr<RHIDevice> m_device;
		std::unique_ptr<RHISwapchain> m_swapchain;
		std::unique_ptr<RHIDescriptorPool> m_descriptor_pool;
	private:

	};
}