#pragma once

#include "Marco.h"
#include "GameObject.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
	class SceneManager final
	{
	public:
		SceneManager();
		~SceneManager();
	private:
		std::vector<GameObject> m_game_objects;
	};
}