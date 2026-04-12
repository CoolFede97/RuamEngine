#pragma once

#include "Camera.h"
#include "nlohmann/json.hpp"
#include "glm/glm.hpp"

namespace glm
{
	inline void to_json(Json& json, const vec4& vector)
	{
		json = Json::array({vector.x, vector.y, vector.z, vector.w});
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
		json = Json::array({vector.x, vector.y, vector.z});
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
		json = Json::array({vector.x, vector.y});
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

namespace RuamEngine
{
	inline void to_json(Json& json, const CameraTransform& cameraTransform)
	{
	    json = Json::array({cameraTransform.pos, cameraTransform.rot});
	}
	inline void from_json(const Json& json, CameraTransform& cameraTransform)
	{
	    if (json.is_array() && json.size() == 2)
		{
		    cameraTransform.pos = json[0].get<glm::vec3>();
			cameraTransform.rot = json[1].get<glm::vec3>();
		}
	}
}
