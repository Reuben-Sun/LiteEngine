#pragma once

#include "Marco.h"
#include "Geometry/Camera.h"
#include "Logic/ECS/LogicScene.h"

namespace ToolEngine
{
	struct MoveState
	{
	public:
		void modify(bool pressed)
		{ 
			int v = pressed ? 1 : -1;
			state = std::clamp(state + v, -1, 1);
		}
		int value() const
		{
			return state;
		}
	private:
		int state = 0;
	};

	class FPSCamera
	{
	public:
		FPSCamera(LogicScene& scene);
		~FPSCamera();

		void updateMoveState(uint32_t keycode, bool pressed);
		void updateRotation(float delta_x, float delta_y);
		void updateCameraSpeed(float delta_speed);
		void setCurrentMousePos(uint32_t x, uint32_t y);

		void tick();

		MoveState m_forward_state;
		MoveState m_right_state;
		MoveState m_up_state;
		
	private:
		LogicScene& m_scene;
		uint32_t m_current_mouse_x;
		uint32_t m_current_mouse_y;
	};
}