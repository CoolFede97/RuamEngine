#include "Skybox.h"
#include "Cubemap.h"
#include "Renderer.h"
#include "RenderingCore.h"
#include "ResourceManager.h"

namespace RuamEngine
{
    GLuint Skybox::m_cubemap = 0;
    MaterialWPtr Skybox::m_material = {};
    RenderUnitSPtr Skybox::m_renderUnit = nullptr;
    bool Skybox::s_initialized = false;

    std::vector<Vertex> Skybox::m_vertices = Vertex::createCube();

    std::vector<unsigned int> Skybox::m_indices = {
        // Back (+Z)
        4, 6, 5,
        6, 4, 7,

        // Front (-Z)
        0, 1, 2,
        2, 3, 0,

        // Left (-X)
        0, 3, 7,
        7, 4, 0,

        // Right (+X)
        1, 5, 6,
        6, 2, 1,

        // Top (+Y)
        3, 2, 6,
        6, 7, 3,

        // Bottom (-Y)
        0, 4, 5,
        5, 1, 0
    };

    void Skybox::SetSkybox(std::vector<std::string>& paths)
    {
        m_cubemap = ResourceManager::LoadTexture<Cubemap>(paths).lock()->rendererIndex();
    }

    void Skybox::Init()
    {
        s_initialized = true;
        m_material = ResourceManager::CreateMaterial();
        m_renderUnit = Renderer::CreateRenderUnit(ShaderProgramType::skybox, m_material);
        m_renderUnit->m_staticPosition = true;
        m_renderUnit->m_staticStorage = true;
        m_renderUnit->pushBatchData(m_vertices, m_indices, {glm::mat4(1.0f)});
        m_material.lock()->m_cubemap = m_cubemap;
    }
}
