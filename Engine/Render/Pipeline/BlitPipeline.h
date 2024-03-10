#pragma once

#include "Marco.h"
#include "RenderPipeline.h"

namespace ToolEngine
{
	class BlitPipeline final : public RenderPipeline
	{
	public:
		BlitPipeline(RHIDevice& device, VkRenderPass render_pass);
		~BlitPipeline();
	private:
		void createPipeline() override;
	};
}