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

	class SaveSystem
	{
	public:
		static Json LoadJsonScene(const std::string& sceneName);
		static void SaveJsonScene(const Json& jsonScene);

		static std::vector<std::string> LoadAllSavedSceneNames();

		static Json LoadJsonRuamConfig();
		static void MakeSureDefaultSceneExists();

		static void SaveRuamConfig();
	};
}
