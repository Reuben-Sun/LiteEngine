#pragma once

#include "Marco.h"
#include "RenderPipeline.h"

namespace ToolEngine
{
	class SkyboxPipeline final: public RenderPipeline
	{
	public:
		SkyboxPipeline(RHIDevice& device, VkRenderPass render_pass);
		~SkyboxPipeline();
	private:
		void createPipeline() override;
	};
}