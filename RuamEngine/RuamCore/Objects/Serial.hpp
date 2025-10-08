#pragma once

#include "SceneManager.hpp"
#include "Scene.hpp"  
#include "nlohmann/json.hpp"

using sm = SceneManager;

using json = nlohmann::json;

void to_json(json& j, Scene* s);
void from_json(const json& j, Scene* s);

void to_json(json& j, Object* o);

class Serial {
	public:
		static void serialise(Scene* s);
		static Scene* deserialise(const std::string& filename);
};