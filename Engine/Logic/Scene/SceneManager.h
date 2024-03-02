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
		SceneManager(RenderScene& scene, const std::string& scene_json = "Scene\\Default.scene");
		~SceneManager();
		
		void tick();
	private:
		std::string m_current_scene = "";

		RenderScene& m_scene;
		LogicScene m_logic_scene;
	};
}