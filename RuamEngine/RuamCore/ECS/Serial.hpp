#pragma once

#include "SceneManager.hpp"
#include "nlohmann/json.hpp"

namespace RuamEngine
{
	using sm = SceneManager;

	using json = nlohmann::json;

	void to_json(json& j, const Scene* s);
	void from_json(const json& j, Scene* s);

	void to_json(json& j, const Entity* o);

	namespace Serial {
	void serialise(Scene* s);
	Scene* deserialise(const std::string& scene_name);
	};
}
