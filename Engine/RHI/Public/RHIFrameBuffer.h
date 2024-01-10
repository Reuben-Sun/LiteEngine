#pragma once

#include "Marco.h"
#include <vulkan/vulkan.h>
#include "RHIDevice.h"

namespace ToolEngine
{
	class RHIFrameBuffer
	{
	public:
		RHIFrameBuffer(RHIDevice& device, VkRenderPass render_pass, VkImageView color_image_view, VkImageView depth_image_view, uint32_t width, uint32_t height);
		~RHIFrameBuffer();

		VkFramebuffer getHandle() const { return m_frame_buffer; }
	private:
		RHIDevice& m_device;
		VkFramebuffer m_frame_buffer;
		VkRenderPass m_render_pass;
		VkImageView m_color_image_view;
		VkImageView m_depth_image_view;
		uint32_t m_width;
		uint32_t m_height;
	};
}