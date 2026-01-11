#pragma once

#include "nlohmann/json.hpp"
#include "glm/glm.hpp"

namespace glm
{
	inline void to_json(Json& json, const vec4& vector)
	{
		json = nlohmann::json::array({vector.x, vector.y, vector.z, vector.w});
	}

	inline void from_json(const Json& json, vec4& vector)
	{
		if (json.is_array() && json.size()>=4)
		{
			vector.x = json[0].get<float>();
			vector.y = json[1].get<float>();
			vector.z = json[2].get<float>();
			vector.w = json[3].get<float>();
		}
	}

	inline void to_json(Json& json, const vec3& vector)
	{
		json = nlohmann::json::array({vector.x, vector.y, vector.z});
	}

	inline void from_json(const Json& json, vec3& vector)
	{
		if (json.is_array() && json.size()>=3)
		{
			vector.x = json[0].get<float>();
			vector.y = json[1].get<float>();
			vector.z = json[2].get<float>();
		}
	}

	inline void to_json(Json& json, const vec2& vector)
	{
		json = nlohmann::json::array({vector.x, vector.y});
	}

	inline void from_json(const Json& json, vec2& vector)
	{
		if (json.is_array() && json.size()>=2)
		{
			vector.x = json[0].get<float>();
			vector.y = json[1].get<float>();
		}
	}
}
