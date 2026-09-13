#include "GizmosManager.h"
#include "RenderingConstants.h"
#include "ResourceManager.h"
#include "Vertex.h"
#include "VertexArray.h"

namespace RuamEngine
{
    SSBOUPtr<GizmoVertex> GizmosManager::s_colliderVertices = nullptr;
    SSBOUPtr<unsigned int> GizmosManager::s_colliderIndices = nullptr;
    VertexArrayUPtr GizmosManager::s_vertexArray = nullptr;
    ShaderProgramSPtr GizmosManager::s_shaderProgram = nullptr;
    unsigned int GizmosManager::s_indexCount = 0;
    unsigned int GizmosManager::s_vertexBaseCount = 0;
    bool GizmosManager::s_inited = false;
    glm::vec4 GizmosManager::s_drawColor = glm::vec4(0, 1, 0, 1);

    void GizmosManager::PushColliderGizmo(std::vector<GizmoVertex>& vertices, std::vector<unsigned int> indices)
    {
        for (unsigned int& index : indices) index += s_vertexBaseCount;
        s_indexCount += indices.size();
        s_vertexBaseCount += vertices.size();
        s_colliderVertices->pushData(vertices);
        s_colliderIndices->pushData(indices);
    }

    void GizmosManager::Init()
    {
        if (s_inited)
        {
            std::cerr << "Error: Trying to init GizmosManager when it was already initialized!\n";
            return;
        }

        s_shaderProgram = ResourceManager::LoadShaderProgram(gizmosVertexShaderDefaultPath, gizmosFragmentShaderDefaultPath);
        s_vertexArray = std::make_unique<VertexArray>();
        s_colliderVertices = std::make_unique<SSBO<GizmoVertex>>(baseVertexCount, GL_DYNAMIC_STORAGE_BIT);
        s_colliderIndices = std::make_unique<SSBO<unsigned int>>(baseIndexCount, GL_DYNAMIC_STORAGE_BIT);
        s_colliderVertices->submitData();
        s_colliderIndices->submitData();
        s_inited = true;
    }
}
