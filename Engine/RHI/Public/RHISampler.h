#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHISampler
	{
	public:
		RHISampler(RHIDevice& device, VkFilter magFilter = VK_FILTER_LINEAR, VkFilter minFilter = VK_FILTER_LINEAR, VkSamplerAddressMode addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT);
		~RHISampler();

		VkSampler getHandle() { return m_sampler; }
	protected:
		VkSampler m_sampler;
		RHIDevice& m_device;
	};
}