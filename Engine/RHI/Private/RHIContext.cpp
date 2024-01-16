#include "RHI/Public/RHIContext.h"

namespace ToolEngine
{
	RHIContext::RHIContext(GlfwWindow& window)
	{
		m_instance = std::make_unique<RHIInstance>(window);
		m_device = std::make_unique<RHIDevice>(*m_instance);
		m_swapchain = std::make_unique<RHISwapchain>(*m_instance, *m_device, window.getWidth(), window.getHeight());
		m_descriptor_pool = std::make_unique<RHIDescriptorPool>(*m_device);
	}
	RHIContext::~RHIContext()
	{
	}
	void RHIContext::tick()
	{
	}
}