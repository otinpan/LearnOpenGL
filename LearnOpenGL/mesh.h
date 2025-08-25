#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"
#include <string>
#include <vector>

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct VertexInfo {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent; // UVのVに対応するベクトル
	glm::vec3 Bitangent; // UVのUに対応するベクトル

	int m_BoneIDs[MAX_BONE_INFLUENCE]; // この頂点に影響を与えるボーンのID
	float m_Weights[MAX_BONE_INFLUENCE]; // 各ボーンがこの頂点をどれだけ動かすかを示す重み
};

struct TextureInfo {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	vector<VertexInfo> vertices;
	vector<unsigned int> indices;
	vector<TextureInfo> textures;
	unsigned int VAO;

	Mesh(vector<VertexInfo> vertices, vector<unsigned int> indices, vector<TextureInfo> textures);
	~Mesh();

	void draw(Shader& shader);

private:
	unsigned int VBO, EBO;

	void setupMesh();
};