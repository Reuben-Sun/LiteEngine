#pragma once

#include "Marco.h"
#include "SceneManager.h"
#include "FPSCamera.h"

namespace ToolEngine
{
	class GPContext
	{
	public:
		GPContext();
		~GPContext();

		RenderScene& getScene() { return m_scene; }
		std::unique_ptr<SceneManager> m_scene_manager;
		std::unique_ptr<FPSCamera> m_fps_camera;

		void tick();
	private:
		RenderScene m_scene;
	};
}