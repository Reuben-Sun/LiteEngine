#pragma once

#include "Marco.h"
#include "Geometry/Camera.h"
#include "Geometry/RenderScene.h"

namespace ToolEngine
{
	class FPSCamera
	{
	public:
		FPSCamera();
		~FPSCamera();

		void tick(RenderScene& scene);
	private:
		Camera m_camera;
	};
}