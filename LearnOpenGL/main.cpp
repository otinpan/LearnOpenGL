#define STB_IMAGE_IMPLEMENTATION

#include"std_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "Common.h"
#include "rectangle.h"
#include "triangle.h"


int main() {
	// GLFW������
	glfwInit();

	// Window�쐬
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LeranOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //window�����݂̃X���b�h�Ŏg����悤�ɂ���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //callback��o�^
	// �E�B���h�E�����T�C�Y���ꂽ�玩����callback���Ă΂��

	// GLAD�̏�����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	float vertices[] = {
		 0.25f, -0.5f, 0.0f, 1.0f,0.0f,1.0f,// left  
		 0.75f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,// right 
		 0.5f,  0.0f, 0.0f,  0.0f,0.0f,1.0f,// top 
	};

	float vertices2[] = {
		//position       // color 
		0.25f,0.0f,0.0f,  1.0f,0.0f,0.0f,
		-0.25f,0.0f,0.0f, 0.0f,1.0f,0.0f,
		0.0f,0.5f,0.0f,   0.0f,0.0f,1.0f
	};


	// Shader�쐬
	Shader mShader("vertex.glsl", "fragment.glsl");
	Shader mShader_yellow("vertex.glsl", "fragment.glsl");

	// �}�`�쐬
	Triangle mTriangle(vertices, sizeof(vertices)/sizeof(float));
	Triangle mTriangle2(vertices2, sizeof(vertices2) / sizeof(float));

	// Texture
	unsigned int texture;
	glGenTextures(1, &texture); // �������ꂽID��texutre�Ɋi�[
	glBindTexture(GL_TEXTURE_2D, texture);

	// Load Texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Assets/container.jpg", &width, &height, &nrChannels, 0);

	// Texture�쐬
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
	glGenerateMipmap(GL_TEXTURE_2D);


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(mShader.getShaderProgram(), "changeColor");
		mShader.use();
		mShader.setBool("useChangeColor", true);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // uniform������ahsder
		mTriangle.draw();

		float xOffset = -0.5;
		float yOffset = -0.5;
		mShader_yellow.use();
		mShader_yellow.setFloat("xOffset", xOffset); //x�����ɕ��s�ړ�
		mShader_yellow.setFloat("yOffset", yOffset); //y�����ɕ��s�ړ�
		mTriangle2.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

