#include "RHI/Public/RHIShader.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
	RHIShader::RHIShader(RHIDevice& device, const std::string& shader_path) :
		m_device(device), m_shader_path(shader_path)
	{
		std::string shader_local_path = Path::getInstance().getCurrentPath() + "/Shaders/" + m_shader_path;
		std::vector<char> shader_code = Path::getInstance().read(shader_local_path);

		VkShaderModuleCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = shader_code.size();
		create_info.pCode = reinterpret_cast<const uint32_t*>(shader_code.data());

		if (vkCreateShaderModule(m_device.getLogicalDevice(), &create_info, nullptr, &m_shader_module) != VK_SUCCESS)
		{
			LOG_ERROR("failed to create shader module!");
		}
		LOG_INFO("load shader: {0}", m_shader_path.c_str());
	}

	RHIShader::~RHIShader()
	{
		if (m_shader_module != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(m_device.getLogicalDevice(), m_shader_module, nullptr);
		}
	}
}