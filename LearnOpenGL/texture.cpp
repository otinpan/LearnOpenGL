#include "texture.h"
#include <iostream>
#include"stb_image.h"

Texture::Texture(const float* vertices, size_t vertexBytes, const unsigned int* indices,  size_t indexBytes,GLenum indexType,
	const std::vector<VertexAttribute>& layout)
	:indexType(indexType)
	,indexCount(calcIndexCount(indexBytes,indexType))
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBytes, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBytes, indices, GL_STATIC_DRAW);

	for (const auto& a : layout) {
		glEnableVertexAttribArray(a.index);
		if (a.integer) {
			glVertexAttribIPointer(a.index, a.size, a.type, a.stride, (const void*)a.offset);
		}
		else {
			glVertexAttribPointer(a.index, a.size, a.type, a.normalized, a.stride, (const void*)a.offset);
		}
	}


	// âèú
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Texture::~Texture() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Texture::initializeTexture(const char* texturePath, int textureUnit) {
	if (textureUnit < 0 || textureUnit >= 16) {
		std::cout << "Only 16 texture are suported" << std::endl;
		return;
	}

	// IDÇtextureÇ…äiî[
	glGenTextures(1, &textures[textureUnit]);
	glBindTexture(GL_TEXTURE_2D, textures[textureUnit]);

	// set the texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load Texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	// TextureçÏê¨
	if (data) {
		GLenum format = (nrChannels == 1) ? GL_RED : (nrChannels == 3) ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

}

void Texture::draw() const {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	for (int i = 0; i < 16; ++i) {
		if (textures[i] != 0) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
		}
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


}


const std::array<unsigned int, 16>& Texture::getTextures() const {
	return textures;
}


GLsizei Texture::calcIndexCount(size_t indexBytes, GLenum indexType) {
	size_t elem = (indexType == GL_UNSIGNED_INT) ? sizeof(unsigned int) :
		(indexType == GL_UNSIGNED_SHORT) ? sizeof(unsigned short) :
		sizeof(unsigned char);
	return static_cast<GLsizei>(indexBytes / elem);
}