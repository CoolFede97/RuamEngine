#include "ModelRenderer.h"
#include "RenderingConstants.h"
#include "RenderingCore.h"
#include "ResourceManager.h"
#include "RuamUtils.h"
#include "RuamEngine.h"
#include <memory>

namespace RuamEngine
{
	ModelRenderer::ModelRenderer(Json modelRendererData, const unsigned int entityId) : Component(entityId)
	{
		if (modelRendererData.contains("m_modelPath")) m_modelPath = modelRendererData["m_modelPath"].get<std::string>();
		loadModel();
	}
    ModelRenderer::~ModelRenderer()
   	{
   		if (!m_modelPath.empty())
   		{
   			m_cachedRenderUnits.clear();
   		}
   	}
    void ModelRenderer::renderUpdate()
	{
		if (!m_model) return;
       	glm::mat4 modelMatrix(1.0f);

       	glm::vec3 globalPosition = transform().globalPosition();
       	glm::vec3 globalRotation = transform().globalRotation();
       	glm::vec3 globalScale = transform().globalScale();

       	modelMatrix = glm::translate(modelMatrix, entity()->transform().position() + globalPosition);
       	modelMatrix = glm::rotate(modelMatrix, glm::radians(entity()->transform().rotation().x + globalRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
       	modelMatrix = glm::rotate(modelMatrix, glm::radians(entity()->transform().rotation().y + globalRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
       	modelMatrix = glm::rotate(modelMatrix, glm::radians(entity()->transform().rotation().z + globalRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
       	modelMatrix = glm::scale(modelMatrix, entity()->transform().scale() * globalScale);

        GetShared(m_matricesSSBO)->pushBatchData({modelMatrix});
	}

	void ModelRenderer::setModel(const std::string& relativePath)
   	{
     	m_modelPath = relativePath;
  		loadModel();
   	}
    void ModelRenderer::loadModel()
   	{
        m_model = ResourceManager::LoadModel(m_modelPath);
        m_modelRU = Renderer::LoadModelRU(m_model);
        auto& ssbo = Renderer::s_modelRUsMap[m_shaderProgramType][m_model->path()];
        if (!ssbo)
        {
            ssbo = std::make_shared<SSBO<glm::mat4>>(maxVertexCount, GL_DYNAMIC_STORAGE_BIT);
        }
        m_matricesSSBO = ssbo;

   	}
    DEF_REGISTER_COMPONENT(ModelRenderer);
}
