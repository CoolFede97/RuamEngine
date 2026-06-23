#include "Skybox.h"
#include "Cubemap.h"
#include "Renderer.h"
#include "RenderingConstants.h"
#include "RenderingCore.h"
#include "ResourceManager.h"
#include "SSBO.h"
#include "ShaderProgram.h"
#include <memory>

namespace RuamEngine
{
    bool Skybox::s_initialized = false;
    ShaderProgramSPtr Skybox::s_shaderProgram;
    VertexArrayUPtr Skybox::m_vertexArray;
    SSBOUPtr<Vertex> Skybox::s_verticesSSBO;
    SSBOUPtr<unsigned int> Skybox::s_indicesSSBO;
    TextureSPtr Skybox::s_cubemap = nullptr;

    std::vector<Vertex> Skybox::s_vertices = Vertex::createCube();

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
        s_cubemap = ResourceManager::LoadTexture<Cubemap>(cubemapPath);
    }
    void Skybox::ChangeCubemap(const std::vector<std::string>& cubemapPaths)
    {
        s_cubemap = ResourceManager::LoadTexture<Cubemap>(cubemapPaths);
    }

    void Skybox::Init()
    {
        s_shaderProgram = ResourceManager::CreateShaderProgram(skyboxVertexShaderDefaultPath, skyboxFragmentShaderDefaultPath);
        m_vertexArray = std::make_unique<VertexArray>();
        s_verticesSSBO = std::make_unique<SSBO<Vertex>>(baseVertexCount, GL_DYNAMIC_STORAGE_BIT);
        s_indicesSSBO = std::make_unique<SSBO<unsigned int>>(baseIndexCount, GL_DYNAMIC_STORAGE_BIT);
        s_initialized = true;
        s_verticesSSBO->pushData(s_vertices);
        s_verticesSSBO->submitData();
        s_indicesSSBO->pushData(s_indices);
        s_indicesSSBO->submitData();
        s_cubemap = ResourceManager::LoadTexture<Cubemap>({
			skyboxDefaultPath,skyboxDefaultPath,skyboxDefaultPath,skyboxDefaultPath,skyboxDefaultPath,skyboxDefaultPath
			});
    }

    void Skybox::Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        s_shaderProgram->updateCameraMatrices(viewMatrix, projectionMatrix);
        m_vertexArray->bind();
        s_verticesSSBO->bindBufferBase(SSBOType::vertices);
        s_indicesSSBO->bindBufferBase(SSBOType::indices);
        GLCall(glActiveTexture(GL_TEXTURE3));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, s_cubemap->glName()));
		GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, s_indicesSSBO->currentSize()/sizeof(unsigned int), 1));
    }
}
