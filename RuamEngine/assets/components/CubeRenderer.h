#pragma once

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"
#include <Component.hpp>
#include "Renderer.h"
#include "Vertex.h"
#include "Buffer.h"

using namespace RuamEngine;

class CubeRenderer : public BaseRenderer
{
	GLuint& textureId;

	void render()
	{
		RenderUnit& genericUnit = Renderer::m_drawingDataMap[Shader::PipelineType::Generic]->m_renderUnits[Material::MaterialType::Generic];
		std::vector<Vertex> newCube = Vertex::CreateCube(0);
		std::vector<unsigned int> newIndices =
		{
			0,1,2, 2,3,0,
			4,5,6, 6,7,4,
			0,1,5, 5,4,0,
			2,3,7, 7,6,2,
			1,2,6, 6,5,1,
			3,0,4, 4,7,3
		};

		if (genericUnit.AddBatchData(newCube, newIndices, { glm::mat4(1.0f) }))
		{
			//indexCount = 0;
		}
	};

	void update()
	{
		render();
	};	
};