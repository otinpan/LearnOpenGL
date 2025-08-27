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

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma); //texture��ǂݍ���id��Ԃ�

class Model {
public:
	Model(string const& path, bool gamma = false);
	~Model();

	void draw(Shader& shader);

	vector<TextureInfo> textures_loaded; //���łɓǂݍ��܂ꂽtexture���i�[
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;

private:
	void loadModel(string const& path); // model��ǂݍ���
	void processNode(aiNode* node, const aiScene* scene); // node���ċA�I�ɏ���
	Mesh processMesh(aiMesh* mesh, const aiScene* scene); // mesh����mesh�N���X���쐬
	vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName); // material����texture��ǂݍ���

};

