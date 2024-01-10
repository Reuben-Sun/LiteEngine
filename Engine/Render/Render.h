#pragma once

#include "Marco.h"
#include "RHI/Public/RHIContext.h"
#include "Render/Pipeline/ForwardPipeline.h"
#include "RHI/Public/RHIFrameBuffer.h"

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
		std::vector<RHIFrameBuffer> m_frame_buffers;
	};
}