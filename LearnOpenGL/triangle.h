#pragma once
#include <glad/glad.h>
#include <vector>

class Triangle {
public:
	Triangle(const float* vertices, size_t vertexCount);
	 Triangle();

	void draw() const;

private:
	unsigned int VAO, VBO;
};