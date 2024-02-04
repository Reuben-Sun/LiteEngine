#pragma once

#include "Marco.h"
#include "GameObject.h"
#include "Core/Path/Path.h"
#include "Geometry/RenderScene.h"

namespace ToolEngine
{
	class SceneManager final
	{
	public:
		SceneManager(RenderScene& scene, const std::string& scene_json = "Scene\\Default.scene");
		~SceneManager();

		void loadScene(const std::string& scene_json);
		void saveScene(const std::string& scene_json);
		
		void tick();
	private:
		std::string m_current_scene = "";
		std::vector<GameObject> m_game_objects;

		RenderScene& m_scene;

		nlohmann::json serialize() const;
		void deserialize(const nlohmann::json& j);
	};
}