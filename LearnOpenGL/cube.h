#pragma once
#include <glad/glad.h>
#include <vector>
#include <array>
#include "vertex_layout.h"

class Cube {
public:
	Cube(const float* vertices, size_t vertexBytes, const unsigned int* indices,
		size_t indexBytes, GLenum indexType, const std::vector<VertexAttribute>& layout);
	~Cube();

	void draw() const;

private:
	unsigned int VAO, VBO, EBO;

	size_t indexCount;
	GLenum indexType;
	GLsizei calcIndexCount(size_t indexBytes, GLenum indexType);

};
