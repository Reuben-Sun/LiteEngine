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
		
		void tick(RenderScene& scene);
	private:
		std::string m_current_scene = "";
		std::vector<GameObject> m_game_objects;

		std::vector<std::string> mesh_name_list;
		std::vector<Mesh> mesh_list;
		std::vector<Transform> mesh_transform_list;
		std::vector<Material> material_list;

		nlohmann::json serialize() const;
		void deserialize(const nlohmann::json& j);
	};
}