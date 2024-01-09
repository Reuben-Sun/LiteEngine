#include "RHI/Public/RHISynchronization.h"

namespace ToolEngine
{
	Fence::Fence(RHIDevice& device) : m_device(device)
	{
		VkFenceCreateInfo fence_info{};
		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		vkCreateFence(m_device.getLogicalDevice(), &fence_info, nullptr, &m_fence);
	}
	Fence::~Fence()
	{
		if (m_fence != VK_NULL_HANDLE)
		{
			vkDestroyFence(m_device.getLogicalDevice(), m_fence, nullptr);
		}
	}
	void Fence::wait()
	{
		vkWaitForFences(m_device.getLogicalDevice(), 1, &m_fence, VK_TRUE, UINT64_MAX);
	}
	void Fence::reset()
	{
		vkResetFences(m_device.getLogicalDevice(), 1, &m_fence);
	}

	Semaphore::Semaphore(RHIDevice& device) : m_device(device)
	{
		VkSemaphoreCreateInfo semaphore_info{};
		semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		vkCreateSemaphore(m_device.getLogicalDevice(), &semaphore_info, nullptr, &m_semaphore);
	}

	Semaphore::~Semaphore()
	{
		if (m_semaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(m_device.getLogicalDevice(), m_semaphore, nullptr);
		}
	}
}