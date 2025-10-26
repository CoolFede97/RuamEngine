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
		RenderUnitPtr genericUnit = Renderer::m_drawingDatas[0]->m_renderUnits[0];
		std::vector<Vertex> newCube = Vertex::CreateCube();
		std::vector<unsigned int> newIndices = 
		{
			0, 1, 2, 2, 3, 0,       // Cara frontal
			4, 5, 6, 6, 7, 4,       // Cara trasera
			8, 9,10,10,11, 8,       // Cara izquierda
		   12,13,14,14,15,12,       // Cara derecha
		   16,17,18,18,19,16,       // Cara inferior
		   20,21,22,22,23,20        // Cara superior
		};


		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, object()->transform().position());
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, object()->transform().scale());

		genericUnit->AddBatchData(newCube, newIndices, { modelMatrix });
	};
	
	
	void start()
	{
	}

	void update()
	{
		BaseRenderer::update();
	};	
};