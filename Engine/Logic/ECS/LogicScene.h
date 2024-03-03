#pragma once

#include "Marco.h"
#include <entt/entt.hpp>
#include "Components.h"

namespace ToolEngine
{
	struct LogicScene
	{
		entt::registry scene_context;
		uint32_t global_id = 0;
		void loadScene(const std::string& path);
		// TODO: save scene
	};
}