#pragma once

#include <vector>
#include "Mesh.h"
#include <string>
#include "Texture.h"
#include "Material.h"
#include "Renderer.h"
#include "Vec3.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace RuamEngine
{
	class Model 
	{
		Model(std::string path);

		std::vector<Mesh> m_meshes;
		std::string m_path;
		unsigned int m_id;
		static unsigned int s_idCount;
		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}