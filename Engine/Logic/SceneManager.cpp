#include "SceneManager.h"

namespace ToolEngine
{
	SceneManager::SceneManager()
	{
		auto j = Path::getInstance().readJson("F:\\go.json");
		GameObject go = GameObject::deserialize(j);
		m_game_objects.push_back(go);
	}
	SceneManager::~SceneManager()
	{
		m_game_objects.clear();
	}
}
