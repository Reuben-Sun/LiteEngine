#pragma once

#include "Marco.h"
#include "RHI/Public/RHIDevice.h"
#include "RHI/Public/RHISwapchain.h"
#include "RHI/Public/RHIPipeline.h"
#include "RHI/Public/RHIPipelineLayout.h"
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

		std::unique_ptr<RHIPipelineLayout> m_pipeline_layout;
		std::unique_ptr<RHIPipeline> m_pipeline;

		void createPipeline();
	};
}