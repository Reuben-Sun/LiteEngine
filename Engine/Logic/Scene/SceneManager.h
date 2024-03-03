#pragma once

#include "Marco.h"
#include "Core/Path/Path.h"
#include "Geometry/RenderScene.h"
#include "Logic/ECS/LogicScene.h"

namespace ToolEngine
{
	class SceneManager final
	{
	public:
		SceneManager(LogicScene& scene, const std::string& scene_json = "Scene\\Default.scene");
		~SceneManager();
		
		void tick(RenderScene& render_scene);
	private:
		std::string m_current_scene = "";

		LogicScene& m_scene;
	};
}