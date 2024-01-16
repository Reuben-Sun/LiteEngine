#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHIDescriptorPool
	{
	public:
		RHIDescriptorPool(RHIDevice& device);
		~RHIDescriptorPool();

		VkDescriptorPool getHandle() const { return m_descriptor_pool; }
	private:
		VkDescriptorPool m_descriptor_pool;
		RHIDevice& m_device;
	};
}