#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "triangle.h"



Triangle::Triangle(const float* vertices, size_t vertexCount)
	:vertexCount(vertexCount)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	// VBO��GPU�ɑ��M
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(float), vertices, GL_STATIC_DRAW);

	// shader�ƒ��_�������тÂ���
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //Layout (location=0)
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // (location=1)
	glEnableVertexAttribArray(1);

	// ����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Triangle::~Triangle() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Triangle::draw() const{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount/3);
}