#include "Skybox.h"
#include "Cubemap.h"
#include "Renderer.h"
#include "RenderingCore.h"
#include "ResourceManager.h"
#include <memory>

namespace RuamEngine
{
    Skybox* Skybox::s_skybox = nullptr;
    GLuint Skybox::m_cubemap = 0;
    MaterialWPtr Skybox::m_material = {};
    RenderUnitSPtr Skybox::m_renderUnit = nullptr;


    std::vector<Vertex> Skybox::m_vertices = Vertex::CreateCube();

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
        s_skybox->m_cubemap = Renderer::RegisterTexture(std::make_shared<Cubemap>(paths));
    }

    void Skybox::update()
    {
        Component::update();
    }

    void Skybox::render()
    {
    }

    void Skybox::start()
    {
        if (s_skybox == nullptr) s_skybox = this;
		else if (s_skybox != this) object()->removeComponent<Skybox>();

        m_material = ResourceManager::CreateMaterial();
        m_renderUnit = Renderer::CreateRenderUnit(Renderer::m_drawingDatas[ShaderProgramType::skybox], m_material);
        m_renderUnit->m_staticPosition = true;
        m_renderUnit->m_staticStorage = true;
        m_renderUnit->AddBatchData(m_vertices, m_indices, {glm::mat4(1.0f)});

        m_material.lock()->m_cubemap = m_cubemap;
    }
}
