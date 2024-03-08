#include "SceneManager.h"
#include "Geometry/GltfLoader.h"

namespace ToolEngine
{
	SceneManager::SceneManager(LogicScene& scene, const std::string& scene_json): m_scene(scene)
	{
		m_scene.loadScene(scene_json);
		// add ui info component
		const auto entity = scene.scene_context.create();
		UIInfoComponent ui_info_component;
		ui_info_component.scene_bounding = glm::vec4(0, 0, 1920, 1080);
		scene.scene_context.emplace<UIInfoComponent>(entity, ui_info_component);
	}
	SceneManager::~SceneManager()
	{
	}
	
	void SceneManager::tick()
	{
		OPTICK_EVENT();
	}
}
