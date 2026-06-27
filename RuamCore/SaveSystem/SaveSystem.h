#pragma once
#include <iostream>
#include "nlohmann/json.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace RuamEngine
{
	inline fs::path scenesDir = fs::path(PROJECT_ROOT_DIR) / "assets" / "scenes";
	inline fs::path configsPath = fs::path(PROJECT_ROOT_DIR) / "config";
	inline fs::path ruamConfigPath = fs::path(PROJECT_ROOT_DIR) / "config" / "ruamConfig.json";

	class Scene;

	class SaveSystem
	{
	public:
		static nlohmann::json LoadJsonScene(const std::string& sceneName);
		static void SaveJsonScene(const nlohmann::json& jsonScene);
		static void SaveCurrentScene();
		static void SaveScene(Scene* scene);
		static void EraseScene(std::string sceneName);
		static std::vector<std::string> LoadAllSavedSceneNames();

		static nlohmann::json LoadJsonRuamConfig();
		static void MakeSureDefaultSceneExists();

		static void SaveRuamConfig();
	};
}
