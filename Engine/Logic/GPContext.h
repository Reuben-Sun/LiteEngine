#pragma once

#include "Marco.h"
#include "SceneManager.h"
#include "Geometry/RenderScene.h"

namespace ToolEngine
{
	class GPContext
	{
	public:
		GPContext();
		~GPContext();

		RenderScene& getScene() { return m_scene; }
		std::unique_ptr<SceneManager> m_scene_manager;

		void tick();
	private:
		RenderScene m_scene;
	};
}