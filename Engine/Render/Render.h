#pragma once

#include "Marco.h"
#include "RHI/Public/RHIContext.h"

namespace ToolEngine
{
	class Renderer
	{
	public:
		Renderer(RHIContext& rhi_context);
		~Renderer();

		void tick();
	private:
		RHIContext& m_rhi_context;
		//std::unique_ptr<ForwardPass> m_render_pass;
		//std::unique_ptr<BlitPipeline> m_blit_pipeline;
	};
}