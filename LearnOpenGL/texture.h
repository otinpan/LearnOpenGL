#pragma once 
#include <glad/glad.h>
#include <vector>
#include <array>
#include "vertex_layout.h"

class Texture {
public:
	Texture(const float* vertices, size_t vertexBytes, const unsigned int* indices, size_t indexBytes, GLenum indexType,
		const std::vector<VertexAttribute>& layout);
	~Texture();

	void draw() const;

	void initializeTexture(const char* texturePath,int textureUnit);

	const std::array<unsigned int, 16>& getTextures() const;


private:
	unsigned int VAO, VBO, EBO;

	size_t indexCount;
	GLenum indexType;
	GLsizei calcIndexCount(size_t indexBytes, GLenum indexType);

	std::array<unsigned int, 16> textures;

};

