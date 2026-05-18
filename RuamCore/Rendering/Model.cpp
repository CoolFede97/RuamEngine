#include "Model.h"
#include "ResourceManager.h"
#include "RuamUtils.h"
#include "Vec3.h"
#include "FileFunctions.h"
#include "assimp/material.h"

#include <memory>
namespace RuamEngine
{
	unsigned int Model::s_idCount = 0;

	Model::Model(std::string path)
		: m_path(GlobalizePath(path)), m_instanceId(s_idCount++)
	{
		loadModel(m_path);
		m_vertices = meshesVertices();
		m_indices = meshesIndices();
	}
	void Model::loadModel(std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
			return;
		}
		processNode(scene->mRootNode, scene);
	}
	void Model::processNode(aiNode* node, const aiScene* scene)
	{

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}


	MeshSPtr Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int  i = 0; i < mesh->mNumVertices; i++)
		{

			Vertex vertex;

			glm::vec3 vector;

			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.m_position = vector;

			if (mesh->mTextureCoords[0])
			{
				vector.x = mesh->mTextureCoords[0][i].x;
				vector.y = mesh->mTextureCoords[0][i].y;
				vertex.m_uv = Vec2(vector.x, vector.y);
			}
			else vertex.m_uv = {0.0f, 0.0f};

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_normal = vector;

			vertices.push_back(vertex);
		}


		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
		}

		MaterialSPtr sharedMeshMaterial = nullptr;

		bool materialFound = false;
		for (auto& pair : m_localToGlobalMaterials)
		{
			if (pair.first == mesh->mMaterialIndex)
			{
				sharedMeshMaterial = pair.second.lock();
				ResourceManager::GetMaterial(sharedMeshMaterial->id());
				materialFound = true;
				break;
			}
		}
		if (!materialFound)
		{
    		std::string relativeDiffusePath = diffuseTexDefaultPath;
    		std::string relativeSpecularPath = specularTexDefaultPath;
    		std::string relativeReflectionPath = reflectionTexDefaultPath;

			m_localToGlobalMaterials.emplace(mesh->mMaterialIndex, sharedMeshMaterial);
    		if (mesh->mMaterialIndex >= 0)
    		{
    			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    			aiString texAiPath;

                tryGetTexturePath(aiTextureType_DIFFUSE, material, texAiPath, relativeDiffusePath);
                tryGetTexturePath(aiTextureType_SPECULAR, material, texAiPath, relativeSpecularPath);
                tryGetTexturePath(aiTextureType_REFLECTION, material, texAiPath, relativeReflectionPath);

                sharedMeshMaterial = GetShared(ResourceManager::CreateMaterial(relativeDiffusePath, relativeSpecularPath, relativeReflectionPath));
    		}
    		else std::cout << "No materials\n";
		}

		return std::make_shared<Mesh>(vertices, indices, sharedMeshMaterial);
	}

	void Model::tryGetTexturePath(aiTextureType texType, aiMaterial* material, aiString& texAiPath, std::string& outRelativePath)
	{
        if (material->GetTexture(texType, 0, &texAiPath) == AI_SUCCESS)
        {
            std::string assimpPath = texAiPath.C_Str();
            if (assimpPath.empty() || assimpPath[0] == '*')
            {
          		std::cout << "This kind of texture path is not supported: " << assimpPath << "\n";
          		ASSERT(false);
            }
            else
            {
          		std::string absoluteModelPath = std::filesystem::path(m_path).parent_path().string();
          		outRelativePath = RelativizePath(absoluteModelPath) + "/" + assimpPath;
            }
        }
	}

	std::vector<Vertex> Model::meshesVertices() const
	{
		std::vector<Vertex> allVertices;
		for (const MeshSPtr& mesh : m_meshes)
		{
			allVertices.insert(allVertices.end(), mesh->m_vertices.begin(), mesh->m_vertices.end());
		}
		return allVertices;
	}
	std::vector<unsigned int> Model::meshesIndices() const
	{
		std::vector<unsigned int> allIndices;
		for (const MeshSPtr& mesh : m_meshes)
		{
			for (unsigned int index : mesh->m_indices)
			{
				allIndices.push_back(index);
			}
		}
		return allIndices;
	}
}
