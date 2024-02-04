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
		SceneManager(const std::string& scene_json = "Scene\\Default.scene");
		~SceneManager();

		void loadScene(const std::string& scene_json);
		void saveScene(const std::string& scene_json);
		
		RenderScene& getScene() { return m_render_scene; }

		void tick();
	private:
		std::string m_current_scene = "";
		std::vector<GameObject> m_game_objects;

		RenderScene m_render_scene;

		nlohmann::json serialize() const;
		void deserialize(const nlohmann::json& j);
	};
}