#pragma once

#include <vector>
#include "Mesh.h"
#include <string>
#include "Texture2D.h"
#include "Material.h"
#include "Renderer.h"
#include "Vec3.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "FileFunctions.h"
#include <algorithm>
#include <unordered_map>
namespace RuamEngine
{
	class Model
	{
	public:
		Model(std::string path);
		std::vector<Mesh> m_meshes;
		std::unordered_map<unsigned int, MaterialPtr> m_localToGlobalMaterials = {};
	private:
		std::string m_path;
		unsigned int m_instanceId;
		static unsigned int s_idCount;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture2D> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		std::vector<Vertex> GetMeshesVertices();
		std::vector<unsigned int> GetMeshesIndices();
	};
	using ModelPtr = std::shared_ptr<Model>;
}
