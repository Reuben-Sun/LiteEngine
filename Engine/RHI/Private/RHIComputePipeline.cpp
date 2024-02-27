#include "RHI/Public/RHIComputePipeline.h"

namespace ToolEngine
{
    RHIComputePipeline::RHIComputePipeline(RHIDevice& device): m_device(device)
    {
        std::vector<RHIDescriptorType> layout_descriptor;
        layout_descriptor.push_back(RHIDescriptorType::StructuredBuffer);
        m_descriptor_set_layout = std::make_unique<RHIDescriptorSetLayout>(m_device, layout_descriptor);
        createPipeline();
        LOG_INFO("Create Compute Pipeline!");
    }
    RHIComputePipeline::~RHIComputePipeline()
    {
    }
    void RHIComputePipeline::createPipeline()
    {
    }
}