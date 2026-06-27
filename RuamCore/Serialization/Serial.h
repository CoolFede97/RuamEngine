#pragma once

#include "RuamConfig.h"
#include "Transform.h"
#include "nlohmann/json.hpp"
namespace fs = std::filesystem;

namespace RuamEngine
{
    class Scene;
	class Transform;
    class Entity;

    using SceneSPtr = std::shared_ptr<Scene>;


    class Serial
    {
    public:
    	static void DeserializeTransform(const nlohmann::json& jsonTransform, Transform* transfrom);

		static nlohmann::json Serialize(const Entity* entity);

		static nlohmann::json Serialize(const Scene* scene);
		static Scene* DeserializeJsonScene(nlohmann::json jsonScene);

		static nlohmann::json Serialize(const RuamConfig& config);
		static RuamConfig DeserializeRuamConfig(const nlohmann::json& jsonConfig);
		static nlohmann::json LoadRuamConfigAsJson();
    };

}
