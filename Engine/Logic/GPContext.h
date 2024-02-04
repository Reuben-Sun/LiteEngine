#pragma once

#include "Marco.h"
#include "SceneManager.h"
#include "FPSCamera.h"
#include "PhysicsManager.h"

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
		std::unique_ptr<PhysicsManager> m_physics_manager;

		void tick();
	private:
		RenderScene m_scene;
	};
}