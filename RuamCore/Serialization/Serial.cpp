#include "Camera.h"
#include "JsonConverters.h"
#include "Serial.h"
#include "Component.h"
#include <exception>
#include "Entity.h"
#include "Transform.h"
#include "SaveSystem.h"
#include "Scene.h"
#include "ModelRenderer.h"
namespace fs = std::filesystem;

namespace RuamEngine
{
	nlohmann::json Serial::Serialize(Component* cmp)
	{
	    nlohmann::json json;
		json["TYPE"] = cmp->name();
		json["m_id"] = cmp->id();

		for (FieldInfo fieldInfo : cmp->fields())
		{
		    json[fieldInfo.name] = fieldInfo.serialize();
		}
		return json;
	}
	nlohmann::json Serial::Serialize(const Entity* entity)
	{
		nlohmann::json jsonEntity =
		{
			{"m_name", entity->name()},
			{"m_id", entity->id()},
			{"m_components", nlohmann::json::array()}
		};
		for (Component* cmp : entity->getComponents())
		{
			nlohmann::json jsonCmp = Serialize(cmp);
			jsonEntity["m_components"].push_back(jsonCmp);
		}
		return jsonEntity;
	}
	nlohmann::json Serial::Serialize(const Scene* scene)
	{
		nlohmann::json jsonScene =
		{
			{"m_name", scene->name()},
			{"m_id", scene->id()},
			{"m_entities", nlohmann::json::array()},
			{"m_lastSavedCameraTransform", scene->m_lastSavedCameraTransform}
		};
		for (Entity* entity : scene->getEntities())
		{
			jsonScene["m_entities"].push_back(Serialize(entity));
		}
		return jsonScene;
	}


	Scene* Serial::DeserializeJsonScene(nlohmann::json jsonScene)
	{
		const unsigned int sceneId = jsonScene["m_id"];

		Scene* scene = new Scene(sceneId, jsonScene["m_name"]);
		scene->m_lastSavedCameraTransform = jsonScene["m_lastSavedCameraTransform"].get<CameraTransform>();
		if (jsonScene["m_entities"].is_null()) return scene;

		for (const nlohmann::json& jsonEntity : jsonScene["m_entities"])
		{
			try
			{
				std::string entityName = jsonEntity["m_name"];
				Entity* entity = scene->createEntity(entityName);
				if (jsonEntity["m_components"].size()<=1) continue;

				for (const nlohmann::json& jsonCmp : jsonEntity["m_components"])
				{
					std::string cmpType = jsonCmp["TYPE"];

					if (!Component::componentRegistry.contains(cmpType))
					{
						std::cerr << "Failed to find the component type " << cmpType << " in the component registry!\n";
						continue;
					}
					auto constructor = Component::componentRegistry[cmpType].addComponent;

					// Uses the constructor defined by the user that takes the Json Component as parameter to add the component to the entity
					Component* cmp = constructor(entity);
					DeserializeJsonComponent(jsonCmp, cmp);
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

	nlohmann::json Serial::Serialize(const RuamConfig& config)
	{
		nlohmann::json jsonConfig =
		{
			{"scenesPaths", nlohmann::json::array()}
		};
		for (auto& sceneName : config.sceneNames)
		{
			jsonConfig["scenesPaths"].push_back(sceneName);
		}
		return jsonConfig;
	};

	void Serial::DeserializeJsonComponent(nlohmann::json jsonCmp, Component* cmp)
	{
	    for (FieldInfo& fieldInfo : cmp->fields())
		{
		    fieldInfo.deserialize(jsonCmp);
		}
	}

	// Make sure everything is okay with the jsonConfig you are passsing as argument (e.g. it has at least one scene)
	RuamConfig Serial::DeserializeRuamConfig(const nlohmann::json& jsonConfig)
	{
		RuamConfig ruamConfig;
		for (std::string sceneName : jsonConfig["scenesPaths"])
		{
			ruamConfig.sceneNames.push_back(sceneName);
		}
		return ruamConfig;
	}

}
