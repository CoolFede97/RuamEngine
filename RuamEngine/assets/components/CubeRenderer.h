#pragma once

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Object.hpp"
#include <Component.hpp>
#include "Renderer.h"
#include "Vertex.h"
#include "RuamTime.h"

using namespace RuamEngine;

class CubeRenderer : public BaseRenderer
{
	using BaseRenderer::BaseRenderer;

	GLuint textureId = 0;

	void render()
	{
		/*RenderUnit& genericUnit = Renderer::m_drawingDataMap[Shader::PipelineType::Generic]->m_renderUnits[Material::MaterialType::Generic];
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

		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, position);
		modelMatrix = glm::scale(modelMatrix, scale);
		modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		if (genericUnit.AddBatchData(newCube, newIndices, { modelMatrix }))*/
	};

	void update()
	{
		render();
		/*
		Object* obj = getOwner(); 
		Transform* transform = obj->getComponent<Transform>();
		transform->setPosition(transform->position() + glm::vec3(0.0,0.0,1.0f*ruamTime::Time::DeltaTime()));*/
	};	
};