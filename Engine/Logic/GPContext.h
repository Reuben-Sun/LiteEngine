#pragma once

#include "Marco.h"
#include "SceneManager.h"

namespace ToolEngine
{
	class GPContext
	{
	public:
		GPContext();
		~GPContext();

		std::unique_ptr<SceneManager> m_scene_manager;
	private:
		
	};
}