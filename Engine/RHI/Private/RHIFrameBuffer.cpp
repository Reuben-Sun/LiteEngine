#include "RHI/Public/RHIFrameBuffer.h"

namespace ToolEngine
{
    RHIFrameBuffer::RHIFrameBuffer(RHIDevice& device, VkRenderPass render_pass, VkImageView color_image_view, uint32_t width, uint32_t height)
        : m_device(device), m_render_pass(render_pass), m_color_image_view(color_image_view), m_width(width), m_height(height)
    {
        std::vector<VkImageView> attachments = { m_color_image_view };

        VkFramebufferCreateInfo frame_buffer_info{};
        frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frame_buffer_info.renderPass = m_render_pass;
        frame_buffer_info.attachmentCount = static_cast<uint32_t>(attachments.size());
        frame_buffer_info.pAttachments = attachments.data();
        frame_buffer_info.width = m_width;
        frame_buffer_info.height = m_height;
        frame_buffer_info.layers = 1;

        if (vkCreateFramebuffer(m_device.getLogicalDevice(), &frame_buffer_info, nullptr, &m_frame_buffer) != VK_SUCCESS)
        {
            LOG_ERROR("failed to create framebuffer!");
        }
        LOG_INFO("Create One Color Frame Buffer");
    }
    RHIFrameBuffer::RHIFrameBuffer(RHIDevice& device, VkRenderPass render_pass, VkImageView color_image_view, VkImageView depth_image_view, uint32_t width, uint32_t height)
        : m_device(device), m_render_pass(render_pass), m_color_image_view(color_image_view), m_depth_image_view(depth_image_view), m_width(width), m_height(height)
	{
        std::vector<VkImageView> attachments = { m_color_image_view, m_depth_image_view };

        VkFramebufferCreateInfo frame_buffer_info{};
        frame_buffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frame_buffer_info.renderPass = m_render_pass;
        frame_buffer_info.attachmentCount = static_cast<uint32_t>(attachments.size());
        frame_buffer_info.pAttachments = attachments.data();
        frame_buffer_info.width = m_width;
        frame_buffer_info.height = m_height;
        frame_buffer_info.layers = 1;

        if (vkCreateFramebuffer(m_device.getLogicalDevice(), &frame_buffer_info, nullptr, &m_frame_buffer) != VK_SUCCESS)
        {
            LOG_ERROR("failed to create framebuffer!");
        }
        LOG_INFO("Create Color with Depth Frame Buffer");
	}
	RHIFrameBuffer::~RHIFrameBuffer()
	{
        if (m_frame_buffer != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(m_device.getLogicalDevice(), m_frame_buffer, nullptr);
        }
	}
}