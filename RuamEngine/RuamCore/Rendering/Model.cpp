#include "Model.h"
#include "Vec3.h"
namespace RuamEngine
{
	unsigned int Model::s_idCount = 0;

	Model::Model(std::string path)
		: m_path(GlobalizePath(path)), m_id(s_idCount++)
	{
		LoadModel(m_path);
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
				vertex.m_uv = Vec2(vector.x, vector.y);
			}
			else vertex.m_uv = {0.0f, 0.0f};	

			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_normal = vector;


			//// Leé esto después Fede
			//std::cout << "Vertex data: --------\n";
			//std::cout << "Position: " << vertex.m_position << "\n";
			//std::cout << "UV: " << vertex.m_uv<< "\n";
			//std::cout << "Normals: " << vertex.m_normal << "\n";


			vertices.push_back(vertex);
		}


		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
		}

		MaterialPtr meshMaterial = nullptr;

		bool materialFound = false;
		for (auto& pair : m_localToGlobalMaterials)
		{
			if (pair.first == mesh->mMaterialIndex)
			{
				meshMaterial = pair.second;
				materialFound = true;
				break;
			}
		}
		if (!materialFound)
		{
			meshMaterial = Renderer::CreateMaterial();
			m_localToGlobalMaterials.emplace(mesh->mMaterialIndex, meshMaterial);
		}

		RenderUnitPtr newRenderUnit = Renderer::CreateRenderUnit(Renderer::m_drawingDatas[0],  meshMaterial);

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			aiString texAiPath;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texAiPath) != AI_SUCCESS)
				std::cout << "Material has no diffuse texture\n";
			else
			{
				std::cout << "HELOOOOOOOOOOOOOOOOOOOO!\n";
				std::string assimpPath = texAiPath.C_Str();
				if (assimpPath.empty() || assimpPath[0] == '*')
				{
					std::cout << "This kind of texture path is not supported: " << assimpPath << "\n";
					ASSERT(false);
				}
				else
				{
					std::string absoluteModelPath = std::filesystem::path(m_path).parent_path().string();
					std::string relativeDiffusePath = RelativizePath(absoluteModelPath) + "/" + assimpPath;
					meshMaterial->m_diffuseIndex = Renderer::CreateTexture(relativeDiffusePath);
				}
			}

			if (material->GetTexture(aiTextureType_SPECULAR, 0, &texAiPath) != AI_SUCCESS)
				std::cout << "Material has no specular texture\n";
			else
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
					std::string relativeSpecularPath = RelativizePath(absoluteModelPath) + "/" + assimpPath;
					meshMaterial->m_specularIndex = Renderer::CreateTexture(relativeSpecularPath);
				}
			}
		}
		else std::cout << "No materials\n";
		return Mesh(vertices, indices, meshMaterial);
	}
	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		return std::vector<Texture>();
	}
}