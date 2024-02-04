#pragma once

#include "Marco.h"
#include "GameObject.h"
#include "Core/Path/Path.h"

namespace ToolEngine
{
	class SceneManager final
	{
	public:
		SceneManager(const std::string& scene_json = "Scene\\Default.scene");
		~SceneManager();

		void loadScene(const std::string& scene_json);
		void saveScene(const std::string& scene_json);
	private:
		std::string m_current_scene;
		std::vector<GameObject> m_game_objects;
		nlohmann::json serialize() const;
		void deserialize(const nlohmann::json& j);
	};
}