#include "RHI/Public/RHISampler.h"

namespace ToolEngine
{
	RHISampler::RHISampler(RHIDevice& device, VkFilter magFilter, VkFilter minFilter, VkSamplerAddressMode addressMode)
		: m_device(device)
	{
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(m_device.getPhysicalDevice(), &properties);

		VkSamplerCreateInfo sampler_info = {};
		sampler_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler_info.magFilter = magFilter;
		sampler_info.minFilter = minFilter;
		sampler_info.addressModeU = addressMode;
		sampler_info.addressModeV = addressMode;
		sampler_info.addressModeW = addressMode;
		sampler_info.anisotropyEnable = VK_TRUE;
		sampler_info.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		sampler_info.unnormalizedCoordinates = VK_FALSE;
		sampler_info.compareEnable = VK_FALSE;
		sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
		sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_info.mipLodBias = 0.0f;
		sampler_info.minLod = 0.0f;
		sampler_info.maxLod = 0.0f;

		if (vkCreateSampler(m_device.getLogicalDevice(), &sampler_info, nullptr, &m_sampler) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create texture sampler!");
		}
	}
	RHISampler::~RHISampler()
	{
		if (m_sampler != VK_NULL_HANDLE)
		{
			vkDestroySampler(m_device.getLogicalDevice(), m_sampler, nullptr);
		}
	}
}