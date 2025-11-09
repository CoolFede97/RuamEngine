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
public:
	using BaseRenderer::BaseRenderer;
	CubeRenderer(const nlohmann::json& j, unsigned int obj_id) : BaseRenderer(obj_id)
	{
	}

public:
	GLuint materialId = 0;
private:
	std::vector<unsigned int> indices = 
	{
		0, 1, 2, 2, 3, 0,       // Cara frontal
		4, 5, 6, 6, 7, 4,       // Cara trasera
		8, 9,10,10,11, 8,       // Cara izquierda
		12,13,14,14,15,12,       // Cara derecha
		16,17,18,18,19,16,       // Cara inferior
		20,21,22,22,23,20        // Cara superior
	};

	std::vector<Vertex> cube = Vertex::CreateCube();
	void render()
	{
		RenderUnitPtr genericUnit = Renderer::m_drawingDatas[0]->m_renderUnits[materialId];

		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, object()->transform().position());
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(object()->transform().rotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::scale(modelMatrix, object()->transform().scale());

		genericUnit->AddBatchData(cube, indices, { modelMatrix });
	};
	
	
	void start()
	{
	}

	void update()
	{
		BaseRenderer::update();
	};
	IMPL_SIMPLE_SERIALIZE(CubeRenderer)
 };
REGISTER_COMPONENT(CubeRenderer)