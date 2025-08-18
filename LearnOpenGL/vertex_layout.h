#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

struct VertexAttribute {
	GLuint index; // layout (location=index) shaderに対応 (例: 0)
	GLint size; // 1つの頂点に含まれる成分数 (例: 3)
	GLenum type; //GL_FLOAT,GL_INT,GL_UNSIGNED_INT, GL_DOUBLEなど (例: GL_FLOAT
	GLboolean normalized; // 正規化されるかどうか (例: GL_FALSE)
	GLsizei stride; // データの頂点間のバイト数 (例: sizeof(float) * 5)
	size_t offset; // 頂点データの開始位置 (例: 0)
	bool integer = false; // 整数属性ならtrue (例: false)
};