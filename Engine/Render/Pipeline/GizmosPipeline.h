#pragma once

#include "Marco.h"
#include "RenderPipeline.h"

namespace ToolEngine
{
	class GizmosPipeline final: public RenderPipeline
	{
	public:
		GizmosPipeline(RHIDevice& device, VkRenderPass render_pass);
		~GizmosPipeline();
	private:
		void createPipeline() override;
	};
}