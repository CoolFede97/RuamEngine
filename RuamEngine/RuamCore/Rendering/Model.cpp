#include "Model.h"
#include "Vec3.h"
namespace RuamEngine
{
	unsigned int Model::s_idCount = 0;
	Model::Model(std::string path)
		: m_path(path), m_id(s_idCount++)
	{
		LoadModel(path);
	}
	void Model::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
			return;
		}
		ProcessNode(scene->mRootNode, scene);
	}
	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}
	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
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
				vertex.m_uv = {x, y};
			}
			else vertex.m_uv = {0.0f, 0.0f};	

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_normal = vector;

			
			// Get vertex data from the mesh and put it into vertices...
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
		}

		MaterialPtr newMaterial = Renderer::CreateMaterial();
		
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiString path;

			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            unsigned int diffuseHandle = Renderer::CreateTexture(std::string(PROJECT_ROOT_DIR) + "/" + m_path + "/" + std::string(path.C_Str()));
			newMaterial->m_diffuseIndex = diffuseHandle;
			
			material->GetTexture(aiTextureType_SPECULAR, 0, &path);
			unsigned int specularIndex = Renderer::CreateTexture(std::string(PROJECT_ROOT_DIR) + "/" + m_path + "/" + std::string(path.C_Str()));
			newMaterial->m_specularIndex = specularIndex;
		}

		return Mesh(vertices, indices, newMaterial);
	}
	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		return std::vector<Texture>();
	}
}