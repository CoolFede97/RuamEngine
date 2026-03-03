#pragma once

#include "RenderUnit.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
//#include "Serial.h"
#include <Component.h>
#include "AudioSystem.h"

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

		static MaterialWPtr m_material;
		static RenderUnitSPtr m_renderUnit;

	public:
		IMPL_SIMPLE_SERIALIZE(Skybox)
		static Skybox* GetSkybox() {return s_skybox; }
		static void SetSkybox(std::vector<std::string>& paths);

		std::string name() override { return "Skybox"; }

		void renderStart() override;
	};
}
