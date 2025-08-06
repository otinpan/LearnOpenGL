#pragma once 
#include <glad/glad.h>
#include <vector>
#include <array>

class Texture {
public:
	Texture(const float* vertices, const unsigned int* indices, size_t vertexCount, size_t indexCount);
	~Texture();

	void draw() const;

	void initializeTexture(const char* texturePath,int textureUnit);

	const std::array<unsigned int, 16>& getTextures() const;


private:
	unsigned int VAO, VBO, EBO;
	size_t vertexCount, indexCount;

	std::array<unsigned int, 16> textures;

};

