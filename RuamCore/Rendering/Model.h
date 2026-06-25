#pragma once


#include "Mesh.h"
#include "Material.h"
#include "FileFunctions.h"
#include "assimp/material.h"

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
	    ~Model();
		Model(const std::string& path);
		const inline std::string relativePath() const { return m_relativePath; }
		const inline std::string globalPath() const { return m_globalPath; }
		std::vector<MeshSPtr> m_meshes;
		std::unordered_map<unsigned int, MaterialWPtr> m_localToGlobalMaterials = {};
		inline const unsigned int instanceId() const { return m_instanceId; }
	private:
		std::string m_relativePath;
		std::string m_globalPath;
		unsigned int m_instanceId;
		static unsigned int s_idCount;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		MeshSPtr processMesh(aiMesh* mesh, const aiScene* scene);

		void tryGetTexturePath(aiTextureType texType, aiMaterial* material, aiString& texAiPath, std::string& outRelativePath);

		std::vector<Vertex> meshesVertices() const;
		std::vector<unsigned int> meshesIndices() const;
	};
	using ModelSPtr = std::shared_ptr<Model>;
	using ModelWPtr = std::weak_ptr<Model>;
}
