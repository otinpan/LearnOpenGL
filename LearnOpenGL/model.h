#pragma once
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include"stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma); //textureを読み込みidを返す

class Model {
public:
	Model(string const& path, bool gamma = false);
	~Model();

	void draw(Shader& shader);

	vector<TextureInfo> textures_loaded; //すでに読み込まれたtextureを格納
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;

private:
	void loadModel(string const& path); // modelを読み込む
	void processNode(aiNode* node, const aiScene* scene); // nodeを再帰的に処理
	Mesh processMesh(aiMesh* mesh, const aiScene* scene); // meshからmeshクラスを作成
	vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName); // materialからtextureを読み込む

};

