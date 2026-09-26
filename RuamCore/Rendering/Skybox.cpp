#include "Skybox.h"
#include "Renderer.h"
#include "RenderingConstants.h"
#include "RenderingCore.h"
#include "ResourceManager.h"
#include "SSBO.h"
#include "ShaderProgram.h"
#include <memory>

namespace RuamEngine
{
    bool Skybox::s_inited = false;
    ShaderProgramSPtr Skybox::s_shaderProgram = nullptr;
    VertexArrayUPtr Skybox::s_vertexArray = nullptr;
    SSBOUPtr<MeshVertex> Skybox::s_verticesSSBO = nullptr;
    SSBOUPtr<unsigned int> Skybox::s_indicesSSBO = nullptr;
    CubemapSPtr Skybox::s_cubemap = nullptr;

    std::vector<MeshVertex> Skybox::s_vertices = MeshVertex::CreateCube();

    std::vector<unsigned int> Skybox::s_indices = {
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

    void Skybox::ChangeCubemap(const std::string& cubemapPath)
    {
        s_cubemap = ResourceManager::LoadCubemap(cubemapPath);
    }
    void Skybox::ChangeCubemap(const std::vector<std::string>& cubemapPaths)
    {
        s_cubemap = ResourceManager::LoadCubemap(cubemapPaths);
    }

    void Skybox::Init()
    {
        if (s_inited)
        {
            std::cerr << "Error: Trying to init Skybox when it was already initialized!\n";
            return;
        }
        s_shaderProgram = ResourceManager::LoadShaderProgram(skyboxVertexShaderPath, skyboxFragmentShaderPath);
        s_vertexArray = std::make_unique<VertexArray>();
        s_verticesSSBO = std::make_unique<SSBO<MeshVertex>>(baseVertexCount, GL_DYNAMIC_STORAGE_BIT);
        s_indicesSSBO = std::make_unique<SSBO<unsigned int>>(baseIndexCount, GL_DYNAMIC_STORAGE_BIT);
        s_inited = true;
        s_verticesSSBO->pushData(s_vertices);
        s_verticesSSBO->submitData();
        s_indicesSSBO->pushData(s_indices);
        s_indicesSSBO->submitData();
        s_cubemap = ResourceManager::LoadCubemap({
			skyboxDefaultPath,skyboxDefaultPath,skyboxDefaultPath,skyboxDefaultPath,skyboxDefaultPath,skyboxDefaultPath
			});
    }
}
