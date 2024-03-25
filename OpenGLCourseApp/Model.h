#pragma once

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.h"
#include "Texture.h"

class Model
{

public:
	Model();
	~Model();
	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

private:
	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);
	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	//lib of textures that we can reuse
	std::vector<unsigned int> meshToTex;
};

