#pragma once


#include "Mesh.h"
#include "Material.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
namespace RuamEngine
{
	class Model
	{
	public:
		Model(std::string path);
		std::vector<MeshSPtr> m_meshes;
		std::unordered_map<unsigned int, MaterialWPtr> m_localToGlobalMaterials = {};
	private:
		std::string m_path;
		unsigned int m_instanceId;
		static unsigned int s_idCount;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		void loadModel(std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		MeshSPtr processMesh(aiMesh* mesh, const aiScene* scene);
		// std::vector<Texture2D> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string& typeName);
		std::vector<Vertex> meshesVertices() const;
		std::vector<unsigned int> meshesIndices() const;
	};
	using ModelSPtr = std::shared_ptr<Model>;
	using ModelWPtr = std::weak_ptr<Model>;
}
