#include "Serial.h"
#include "Component.h"
#include <exception>
#include <fstream>
#include <filesystem>
#include "Entity.h"
#include "SceneManager.h"
#include "Transform.h"
#include "SaveSystem.h"

namespace fs = std::filesystem;

namespace RuamEngine
{
	void Serial::DeserializeTransform(const Json& jsonTransform, Transform& transfrom)
	{
		if (jsonTransform.contains("m_position")) transfrom.setPosition(jsonTransform["m_position"].get<glm::vec3>());
		if (jsonTransform.contains("m_rotation")) transfrom.setRotation(jsonTransform["m_rotation"].get<glm::vec3>());
		if (jsonTransform.contains("m_scale")) transfrom.setScale(jsonTransform["m_scale"].get<glm::vec3>());
	}

	Json Serial::Serialize(const Entity* entity)
	{
		Json jsonEntity =
		{
			{"m_name", entity->name()},
			{"m_id", entity->id()},
			{"m_components", Json::array()}
		};
		for (Component* cmp : entity->getComponents())
		{
			Json jsonCmp = *cmp;
			jsonEntity["m_components"].push_back(jsonCmp);
		}
		return jsonEntity;
	}
	Json Serial::Serialize(const Scene* scene)
	{
		Json jsonScene =
		{
			{"m_name", scene->name()},
			{"m_id", scene->id()},
			{"m_entities", Json::array()}
		};
		for (Entity* entity : scene->getEntities())
		{
			jsonScene["m_entities"].push_back(Serialize(entity));
		}
		return jsonScene;
	}


	Scene* Serial::DeserializeJsonScene(Json jsonScene)
	{
		const unsigned int sceneId = jsonScene["m_id"];

		Scene* scene = new Scene(sceneId, jsonScene["m_name"]);

		if (jsonScene["m_entities"].is_null()) return scene;

		for (const Json& jsonEntity : jsonScene["m_entities"])
		{
			try
			{
				std::string entityName = jsonEntity["m_name"];
				Entity* entity = scene->createEntity(entityName);
				if (jsonEntity["m_components"].size()<=1) continue;

				for (const Json& jsonCmp : jsonEntity["m_components"])
				{
					std::string cmpType = jsonCmp["TYPE"];

					if (!Component::componentRegistry.contains(cmpType))
					{
						std::cerr << "Failed to find the component type " << cmpType << " in the component registry!\n";
						continue;
					}
					if (cmpType != "Transform")
					{
						auto constructor = Component::componentRegistry[cmpType].addComponentWithJson;

						// Uses the constructor defined by the user that takes the Json Component as parameter to add the component to the entity
						constructor(jsonCmp, entity);
					}
					else
					{
						DeserializeTransform(jsonCmp, entity->transform());
					}
				}
			}
			catch (const std::exception& exception)
			{
				std::cerr << "Error loading component: " << exception.what() << "\n";
				continue;
			}
		}
		return scene;
	}

	Json Serial::Serialize(const RuamConfig& config)
	{
		Json jsonConfig =
		{
			{"scenesPaths", Json::array()}
		};
		for (auto& sceneName : config.sceneNames)
		{
			jsonConfig["scenesPaths"].push_back(sceneName);
		}
		return jsonConfig;
	};

	// Make sure everything is okay with the jsonConfig you are passsing as argument (e.g. it has at least one scene)
	RuamConfig Serial::DeserializeRuamConfig(const Json& jsonConfig)
	{
		RuamConfig ruamConfig;
		for (std::string sceneName : jsonConfig["scenesPaths"])
		{
			ruamConfig.sceneNames.push_back(sceneName);
		}
		return ruamConfig;
	}

}
