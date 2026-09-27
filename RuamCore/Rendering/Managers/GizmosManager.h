#pragma once

#include "ShaderProgram.h"
#include "Vertex.h"
#include "SSBO.h"
#include "VertexArray.h"

namespace RuamEngine
{
    class GizmosManager
    {
    public:
        static VertexArrayUPtr s_vertexArray;
        static SSBOUPtr<GizmoVertex> s_colliderVertices;
        static SSBOUPtr<unsigned int> s_colliderIndices;
        static ShaderProgramSPtr s_shaderProgram;
        static glm::vec4 s_drawColor;
        static unsigned int s_indexCount;
        static unsigned int s_vertexBaseCount;

        static void Init();
        static void PushColliderGizmo(std::vector<GizmoVertex>& vertices, std::vector<unsigned int> indices);
    private:
        static bool s_inited;
    };
}
