#pragma once

#include "Marco.h"
#include "RenderPipeline.h"

namespace ToolEngine
{
	class OutlinePipeline final : public RenderPipeline
	{
	public:
		OutlinePipeline(RHIDevice& device, VkRenderPass render_pass);
		~OutlinePipeline();
	private:
		void createPipeline() override;
	};
}