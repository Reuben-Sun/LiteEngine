#pragma once

#include "Marco.h"
#include "Scene/SceneManager.h"
#include "Camera/FPSCamera.h"
#include "Physics/PhysicsManager.h"
#include "Logic/ECS/LogicScene.h"

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
		LogicScene m_logic_scene;
	};
}