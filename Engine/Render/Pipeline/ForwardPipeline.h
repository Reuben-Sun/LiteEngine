#pragma once

#include "Marco.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHISwapchain.h"
#include "RHI/Public/RHIPipeline.h"

namespace ToolEngine
{
	class ForwardPipeline final
	{
	public:
		ForwardPipeline(RHIDevice& device, RHISwapchain& swapchain, uint32_t frames_count);
		~ForwardPipeline();

		//void tick();
	private:
		RHIDevice& m_device;
		RHISwapchain& m_swapchain;
		uint32_t m_frames_count;

		void createPipeline();
	};
}