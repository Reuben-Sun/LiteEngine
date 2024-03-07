#include "SceneManager.h"
#include "Geometry/GltfLoader.h"

namespace ToolEngine
{
	SceneManager::SceneManager(LogicScene& scene, const std::string& scene_json): m_scene(scene)
	{
		m_scene.loadScene(scene_json);
		
	}
	SceneManager::~SceneManager()
	{
	}
	
	void SceneManager::tick()
	{
		OPTICK_EVENT();
	}
}
