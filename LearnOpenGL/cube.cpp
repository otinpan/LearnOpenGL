#include "cube.h"
#include <iostream>

Cube::Cube(const float* vertices, size_t vertexBytes, const unsigned int* indices,
	size_t indexBytes, GLenum indexType, const std::vector<VertexAttribute>& layout)
	: indexType(indexType), indexCount(calcIndexCount(indexBytes, indexType)) {
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

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Cube::~Cube() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Cube::draw() const {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), indexType, 0);
}

GLsizei Cube::calcIndexCount(size_t indexBytes, GLenum indexType) {
	size_t elem = (indexType == GL_UNSIGNED_INT) ? sizeof(unsigned int) :
		(indexType == GL_UNSIGNED_SHORT) ? sizeof(unsigned short) :
		sizeof(unsigned char);
	return static_cast<GLsizei>(indexBytes / elem);
}