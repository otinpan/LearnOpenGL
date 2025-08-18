#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

struct VertexAttribute {
	GLuint index; // layout (location=index) shader�ɑΉ� (��: 0)
	GLint size; // 1�̒��_�Ɋ܂܂�鐬���� (��: 3)
	GLenum type; //GL_FLOAT,GL_INT,GL_UNSIGNED_INT, GL_DOUBLE�Ȃ� (��: GL_FLOAT
	GLboolean normalized; // ���K������邩�ǂ��� (��: GL_FALSE)
	GLsizei stride; // �f�[�^�̒��_�Ԃ̃o�C�g�� (��: sizeof(float) * 5)
	size_t offset; // ���_�f�[�^�̊J�n�ʒu (��: 0)
	bool integer = false; // ���������Ȃ�true (��: false)
};