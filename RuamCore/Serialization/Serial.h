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
    	static void DeserializeTransform(const Json& jsonTransform, Transform& transfrom);

		static Json Serialize(const Entity* entity);

		static Json Serialize(const Scene* scene);
		static Scene* DeserializeJsonScene(Json jsonScene);

		static Json Serialize(const RuamConfig& config);
		static RuamConfig DeserializeRuamConfig(const Json& jsonConfig);
		static Json LoadRuamConfigAsJson();
    };

}
