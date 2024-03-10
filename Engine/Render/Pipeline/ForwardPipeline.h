#pragma once

#include "Marco.h"
#include "RenderPipeline.h"

namespace ToolEngine
{
	class ForwardPipeline final : public RenderPipeline
	{
	public:
		ForwardPipeline(RHIDevice& device, VkRenderPass render_pass);
		~ForwardPipeline();
	private:
		void createPipeline() override;
	};
}