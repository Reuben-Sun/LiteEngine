#pragma once

#include "Marco.h"
#include "RenderPipeline.h"

namespace ToolEngine
{
	class TransparentPipeline final : public RenderPipeline
	{
	public:
		TransparentPipeline(RHIDevice& device, VkRenderPass render_pass);
		~TransparentPipeline();
	private:
		void createPipeline() override;
	};
}