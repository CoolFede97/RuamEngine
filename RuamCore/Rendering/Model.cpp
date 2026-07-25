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

	Model::~Model()
	{
        ResourceManager::RemoveModelIfExpired(m_relativePath);
	}

	Model::Model(const std::string& path)
		: m_relativePath(path), m_globalPath(globalizePath(path)), m_instanceId(s_idCount++)
	{
		loadModel(m_globalPath);
	}
	void Model::loadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
			return;
		}
		aiMatrix4x4 identityMatrix;
		processNode(scene->mRootNode, scene, identityMatrix);
	}
	void Model::processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 parentTransform)
	{
	    aiMatrix4x4 globalPos = parentTransform * node->mTransformation;
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(processMesh(mesh, scene, globalPos));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, globalPos);
		}
	}


	MeshSPtr Model::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 globalPos)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int  i = 0; i < mesh->mNumVertices; i++)
		{

			Vertex vertex;

			aiVector3D pos = globalPos * mesh->mVertices[i];
			vertex.m_position = { pos.x, pos.y, pos.z };

			glm::vec3 vector;

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
            Texture2DSPtr diffuse;
            Texture2DSPtr specular;
            Texture2DSPtr reflection;
    		if (mesh->mMaterialIndex >= 0)
    		{
    			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    			aiString texAiPath;

                diffuse = getTexture2D(aiTextureType_DIFFUSE, material, texAiPath, *scene);
                specular = getTexture2D(aiTextureType_SPECULAR, material, texAiPath, *scene);
                reflection = getTexture2D(aiTextureType_REFLECTION, material, texAiPath, *scene);

                sharedMeshMaterial = ResourceManager::CreateMaterial(diffuse, specular, reflection);
                m_localToGlobalMaterials.emplace(mesh->mMaterialIndex, sharedMeshMaterial);
    		}
    		else std::cout << "No materials\n";
		}

		return std::make_shared<Mesh>(vertices, indices, sharedMeshMaterial);
	}

	Texture2DSPtr Model::getTexture2D(aiTextureType texType, aiMaterial* material, aiString& texAiPath, const aiScene& scene)
	{
        if (material->GetTexture(texType, 0, &texAiPath) == AI_SUCCESS)
        {
            std::string assimpPath = texAiPath.C_Str();
            if (assimpPath.empty())
            {
          		std::cout << "Assimp path empty when getting texture from model\n";
          		ASSERT(false);
            }
            else if (assimpPath[0] == '*')
            {
                const aiTexture* aiTex = scene.GetEmbeddedTexture(texAiPath.C_Str());
                return ResourceManager::LoadTexture2D(m_relativePath+assimpPath, aiTex);
            }
            else
            {
          		std::string absoluteModelPath = std::filesystem::path(m_globalPath).parent_path().string();
                return ResourceManager::LoadTexture2D(relativizePath(absoluteModelPath) + "/" + assimpPath);
            }
        }
        return ResourceManager::LoadTexture2D(specularTexDefaultPath);
	}
}
