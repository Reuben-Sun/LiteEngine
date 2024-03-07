#pragma once

#include "Marco.h"

#include "RHI/Public/RHIContext.h"
#include "Render/Editor/EditorUI.h"
#include "Render/Runtime/Render.h"
#include "Render/Pass/UIPass.h"
#include "RHI/Public/RHIFrameBuffer.h"
#include "RHI/Public/RHICommandBuffer.h"
#include "RHI/Public/RHIDescriptorSet.h"
#include "RHI/Public/RHITextureImage.h"
#include "Geometry/RenderScene.h"
#include "Logic/ECS/LogicScene.h"

namespace ToolEngine
{
	class RenderContext
	{
	public:
		RenderContext(RHIContext& rhi_context);
		~RenderContext();

		void tick(LogicScene& scene);

		void setFullScreen();
	private:
		RHIContext& m_rhi_context;
		RenderScene m_scene;
		
		std::unique_ptr<EditorUI> m_editor_ui;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<RHICommandBuffer> m_command_buffer;
		UIContext m_ui_context;		
		
		std::vector<std::unique_ptr<Fence>> m_in_flight_fences;
		std::vector<std::unique_ptr<Semaphore>> m_image_available_semaphores;
		std::vector<std::unique_ptr<Semaphore>> m_render_finished_semaphores;

		uint32_t m_max_frames_in_flight{ 3 };
		uint32_t m_current_frame{ 0 };
		uint32_t getFrameIndex() const { return m_current_frame % m_max_frames_in_flight; }

		bool prepareFrame(uint32_t& image_index);
		void submitFrame(uint32_t image_index);
	};
}