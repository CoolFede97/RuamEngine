#pragma once

#include "SceneManager.h"
#include "nlohmann/json.hpp"
#include <filesystem>
namespace fs = std::filesystem;

namespace RuamEngine
{
    class Entity;

	inline fs::path scenesDir = fs::path(PROJECT_ROOT_DIR) / "assets" / "scenes" / "savedScenes";

	Json serialize(const Entity* entity);
	Json serialize(const Scene* scene);

	Scene* deserialize(const std::string& sceneName);

}
