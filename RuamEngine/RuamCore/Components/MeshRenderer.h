#pragma once

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"
#include <Component.hpp>
#include "Renderer.h"
#include "Vertex.h"
#include "RuamTime.h"
#include <string>
#include "Model.h"
using namespace RuamEngine;

class CubeRenderer : public BaseRenderer
{
	using BaseRenderer::BaseRenderer;

	std::string m_meshPath = "Assets/Meshes/Bag.obj"; // FEDE MIRALO DESPUÉS!!!
	Model m_model;

	void render()
	{
		RenderUnit& genericUnit = Renderer::m_drawingDataMap[Shader::PipelineType::Generic]->m_renderUnits[Material::MaterialType::Generic];
	
	};


	void start()
	{
		m_model = Model(m_meshPath);
	}

	void update()
	{
		BaseRenderer::update();
	};
};