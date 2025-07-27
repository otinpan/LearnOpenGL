#pragma once
#include <glad/glad.h>
#include <vector>

class Rectangle {
public:
	Rectangle(const float* vertices,const int* indices, size_t vertexCount);
	~Rectangle();

	void draw() const;
private:
	unsigned int VAO, VBO, EBO;
};