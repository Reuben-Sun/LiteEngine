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

		float m_fix_update_rate = 60.0f;
		float getFixedDeltaTime() const { return 1.0f / m_fix_update_rate; }
		float m_accumulator = 0.0f;
	};
}