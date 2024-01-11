#pragma once

#include "Marco.h"
#include "RHI/Public/RHIContext.h"
#include "RHI/Public/RHIFrameBuffer.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "Render/Pipeline/ForwardPipeline.h"
#include "Render/Resources/DepthResources.h"

namespace ToolEngine
{
	class Renderer
	{
	public:
		Renderer(RHIContext& rhi_context);
		~Renderer();

		const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

		void tick();
	private:
		RHIContext& m_rhi_context;
		std::unique_ptr<ForwardPipeline> m_forward_pipeline;
		std::unique_ptr<DepthResources> m_depth_resources;
		std::vector<std::unique_ptr<RHIFrameBuffer>> m_frame_buffers;
		std::unique_ptr<RHICommandBuffer> m_command_buffer;
	};
}