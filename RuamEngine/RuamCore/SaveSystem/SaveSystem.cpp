#include "SaveSystem.h"

#include "RuamConfig.h"
#include "SceneManager.h"
#include "Serial.h"
#include "Engine.h"

#include <exception>
#include <filesystem>
#include <fstream>

namespace RuamEngine
{
	Json SaveSystem::LoadJsonScene(const std::string& sceneName)
	{
		std::string fileName = sceneName + ".json";
		fs::path filePath = scenesDir / fileName;

		if (!fs::exists(filePath))
		{
			std::cerr << "Failed to find scene: " << filePath << " before deserializing!\n";
			return {};
		}
		std::ifstream file(filePath);
		Json jsonScene;
		file >> jsonScene;

		return jsonScene;
	}

	void SaveSystem::SaveJsonScene(const Json &jsonScene)
	{
		std::cout << "KQODFKWOFKWEF\n";
		fs::create_directories(scenesDir);

		std::string fileName = jsonScene["m_name"].get<std::string>() + ".json";
		fs::path filePath = scenesDir / fileName;

		std::ofstream file(filePath);
		if (!file)
		{
			std::cerr << "Failed to open file: " << filePath << " when saving scene of name " << jsonScene["m_name"] << "\n";
		}
		file << jsonScene.dump(1);
		std::cout << "LISTOOO\n";
		file.close();
	}

	void SaveSystem::SaveCurrentScene()
	{
		SaveJsonScene(Serial::Serialize(SceneManager::ActiveScene()));
		std::cout << "Changes saved successfully!\n";
	}

	void SaveSystem::SaveScene(Scene* scene)
	{
		SaveJsonScene(Serial::Serialize(scene));
	}

	std::vector<std::string> SaveSystem::LoadAllSavedSceneNames()
	{
		std::vector<std::string> sceneNames;

		std::filesystem::create_directories(scenesDir);

		for (const auto& entry : fs::directory_iterator(scenesDir))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".json")
			{
				try
				{
					std::ifstream file(entry.path());
					Json jsonScene;
					file >> jsonScene;

					if (jsonScene.contains("m_id") && jsonScene.contains("m_name"))
					{
						sceneNames.push_back(jsonScene["m_name"].get<std::string>());
					}
				}
				catch (const std::exception& e)
				{
					std::cerr << "Warning: Failed to parse potential scene file: " <<
					entry.path() << " | " << e.what() << "\n";
				}
			}
		}
		return sceneNames;
	}

	Json SaveSystem::LoadJsonRuamConfig()
	{
		if (!fs::exists(ruamConfigPath))
		{
			std::cerr << "Failed to find ruamConfig at path: " << ruamConfigPath << "!\n";
			RuamConfig ruamConfig;
			MakeSureDefaultSceneExists();
			ruamConfig.sceneNames.push_back("defaultScene");
			return Serial::Serialize(ruamConfig);
		}
		else
		{
			std::ifstream file(ruamConfigPath);
			Json jsonConfig;
			file >> jsonConfig;
			return jsonConfig;
		}
	}

	void SaveSystem::MakeSureDefaultSceneExists()
	{
		if (!fs::exists(scenesDir / "defaultScene.json"))
		{
			SceneSPtr defaultScene = SceneManager::CreateDefaultScene();
			SaveJsonScene(Serial::Serialize(defaultScene.get()));
		}
	}

	void SaveSystem::SaveRuamConfig()
	{
		if (!fs::exists(configsPath))
		{
			std::cerr << "Directory " << configsPath << " not found. This directory is about to be created automatically now\n";
			fs::create_directories(configsPath);
		}
		if (!fs::exists(ruamConfigPath))
		{
			std::cerr << "Failed to find ruamConfig at path: " << ruamConfigPath << "! A new one is about to be created now\n";
			RuamConfig ruamConfig;
			Json jsonRuamConfig = Serial::Serialize(ruamConfig);

			std::ofstream file(ruamConfigPath);
			if (!file)
			{
				std::cerr << "Failed to open file: " << ruamConfigPath << " when trying to save ruam config\n";
			}
			file << jsonRuamConfig.dump(1);
			file.close();
		}
		else
		{
			std::ofstream file(ruamConfigPath);
			if (!file)
			{
				std::cerr << "Failed to open file: " << ruamConfigPath << " when trying to save ruam config\n";
			}
			Json jsonRuamConfig = Serial::Serialize(Engine::Config());
			file << jsonRuamConfig.dump(1);
			file.close();
		}
	}
}
