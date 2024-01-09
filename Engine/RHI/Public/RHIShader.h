#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHIShader
	{
	public:
		RHIShader(RHIDevice& device, const std::string& shader_path);
		~RHIShader();
		VkShaderModule getHandle() const { return m_shader_module; }
	private:
		VkShaderModule m_shader_module;
		std::string m_shader_path;
		RHIDevice& m_device;
	};
}