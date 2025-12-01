#include "Skybox.h"
#include "Cubemap.h"

namespace RuamEngine
{
    Skybox* Skybox::s_skybox = nullptr;

    std::vector<float> Skybox::m_skyboxVertices =
    {
        // Back face (positive Z)
        -1.0f, -1.0f,  1.0f,  // Bottom-left
        1.0f,  1.0f,  1.0f,  // Top-right
        1.0f, -1.0f,  1.0f,  // Bottom-right
        1.0f,  1.0f,  1.0f,  // Top-right
        -1.0f, -1.0f,  1.0f,  // Bottom-left
        -1.0f,  1.0f,  1.0f,  // Top-left

        // Front face (negative Z)
        -1.0f, -1.0f, -1.0f,  // Bottom-left
        1.0f, -1.0f, -1.0f,  // Bottom-right
        1.0f,  1.0f, -1.0f,  // Top-right
        1.0f,  1.0f, -1.0f,  // Top-right
        -1.0f,  1.0f, -1.0f,  // Top-left
        -1.0f, -1.0f, -1.0f,  // Bottom-left

        // Left face (negative X)
        -1.0f,  1.0f,  1.0f,  // Top-right
        -1.0f,  1.0f, -1.0f,  // Top-left
        -1.0f, -1.0f, -1.0f,  // Bottom-left
        -1.0f, -1.0f, -1.0f,  // Bottom-left
        -1.0f, -1.0f,  1.0f,  // Bottom-right
        -1.0f,  1.0f,  1.0f,  // Top-right

        // Right face (positive X)
        1.0f,  1.0f,  1.0f,  // Top-left
        1.0f, -1.0f, -1.0f,  // Bottom-right
        1.0f,  1.0f, -1.0f,  // Top-right
        1.0f, -1.0f, -1.0f,  // Bottom-right
        1.0f,  1.0f,  1.0f,  // Top-left
        1.0f, -1.0f,  1.0f,  // Bottom-left

        // Top face (positive Y)
        -1.0f,  1.0f, -1.0f,  // Top-left
        1.0f,  1.0f,  1.0f,  // Bottom-right
        1.0f,  1.0f, -1.0f,  // Top-right
        1.0f,  1.0f,  1.0f,  // Bottom-right
        -1.0f,  1.0f, -1.0f,  // Top-left
        -1.0f,  1.0f,  1.0f,  // Bottom-left

        // Bottom face (negative Y)
        -1.0f, -1.0f, -1.0f,  // Top-right
        1.0f, -1.0f, -1.0f,  // Top-left
        1.0f, -1.0f,  1.0f,  // Bottom-left
        1.0f, -1.0f,  1.0f,  // Bottom-left
        -1.0f, -1.0f,  1.0f,  // Bottom-right
        -1.0f, -1.0f, -1.0f   // Top-right
    };

    void Skybox::SetSkybox(std::vector<std::string> paths)
    {
        s_skybox->m_cubemap = new Cubemap(paths);
    }

    void Skybox::update()
    {
        Component::update();
    }

    void Skybox::start()
    {
        if (s_skybox == nullptr) s_skybox = this;
		else if (s_skybox != this) object()->removeComponent<Skybox>();
    }
}
