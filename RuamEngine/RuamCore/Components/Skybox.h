#pragma once

#include "RenderUnit.h"
#include "Renderer.h"
#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"
#include "Serial.hpp"
#include <Component.hpp>
#include "AudioSystem.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Cubemap.h"

namespace RuamEngine
{
	class Skybox : public Component
	{
		using Component::Component;

		static Skybox* s_skybox;
		static GLuint m_cubemap;

		static std::vector<Vertex> m_vertices;
		static std::vector<unsigned int> m_indices;

		static MaterialPtr m_material;
		static RenderUnitPtr m_renderUnit;

	public:
		Skybox(const nlohmann::json& j, const unsigned int obj_id);

		static Skybox* GetSkybox() {return s_skybox; }
		static void SetSkybox(std::vector<std::string> paths);
		void update();
		void start();
		void render();

		//IMPL_SERIALIZE(Skybox,
		// SER_FIELD(m_cubemap)
		//);
	};
}
