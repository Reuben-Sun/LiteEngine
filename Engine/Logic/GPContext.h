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

		LogicScene& getLogicScene() { return m_logic_scene; }
		std::unique_ptr<SceneManager> m_scene_manager;
		std::unique_ptr<FPSCamera> m_fps_camera;
		std::unique_ptr<PhysicsManager> m_physics_manager;

		void tick();
		void setCurrentMousePosition(uint32_t x, uint32_t y);
	private:
		LogicScene m_logic_scene;
	};
}