#pragma once

#include "Marco.h"
#include "RHI/Public/RHIPipeline.h"
#include "RHI/Public/RHIDevice.h"

namespace ToolEngine
{
	class GraphicsPipeline : public RHIPipeline
	{
	public:
		GraphicsPipeline(RHIDevice& device, RHIPipelineState pipeline_state);
		virtual ~GraphicsPipeline() = default;
	};
}