#include "SceneManager.h"

namespace ToolEngine
{
	SceneManager::SceneManager(const std::string& scene_json): m_current_scene(scene_json)
	{
		loadScene(scene_json);
	}
	SceneManager::~SceneManager()
	{
		m_game_objects.clear();
	}
	void SceneManager::loadScene(const std::string& scene_json)
	{
		if (scene_json != m_current_scene)
		{
			m_current_scene = scene_json;
			std::string file_path = Path::getInstance().getAssetPath() + scene_json;
			auto j = Path::getInstance().readJson(file_path);
			deserialize(j);
		}
	}
	void SceneManager::saveScene(const std::string& scene_json)
	{
		std::string file_path = Path::getInstance().getAssetPath() + scene_json;
		auto j = serialize();
		Path::getInstance().saveJson(file_path, j);
	}
	nlohmann::json SceneManager::serialize() const
	{
		nlohmann::json scene;
		std::vector<nlohmann::json> game_objects;
		for (const auto& go : m_game_objects)
		{
			game_objects.push_back(go.serialize());
		}
		scene["game_objects"] = game_objects;

		return scene;
	}
	void SceneManager::deserialize(const nlohmann::json& j)
	{
		m_game_objects.clear();
		auto game_objects = j.at("game_objects").get<std::vector<nlohmann::json>>();
		for (const auto& go : game_objects)
		{
			m_game_objects.push_back(GameObject::deserialize(go));
		}
	}
}
