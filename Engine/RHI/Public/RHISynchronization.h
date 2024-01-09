#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class Fence
	{
	public:
		Fence(RHIDevice& device);
		~Fence();

		VkFence getHandle() const { return m_fence; }
		void wait();
		void reset();
	private:
		VkFence m_fence{ VK_NULL_HANDLE };
		RHIDevice& m_device;
	};

	class Semaphore
	{
	public:
		Semaphore(RHIDevice& device);
		~Semaphore();

		VkSemaphore getHandle() const { return m_semaphore; }
	private:
		VkSemaphore m_semaphore{ VK_NULL_HANDLE };
		RHIDevice& m_device;
	};
}