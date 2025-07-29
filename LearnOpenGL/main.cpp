#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "Common.h"
#include "rectangle.h"
#include "triangle.h"


int main() {
	// GLFW初期化
	glfwInit();

	// Window作成
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LeranOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //windowを現在のスレッドで使えるようにする
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //callbackを登録
	// ウィンドウがリサイズされたら自動でcallbackが呼ばれる

	// GLADの初期化
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	float vertices[] = {
		 0.25f, -0.5f, 0.0f, // left  
		 0.75f, -0.5f, 0.0f, // right 
		 0.5f,  0.0f, 0.0f,  // top 

		-0.75f,-0.5f,0.0f, //left
		-0.5f,0.0f,0.0f, //right
		-0.25f,-0.5f,0.0f, //top
	};

	float vertices2[] = {
		0.25f,0.0f,0.0f,
		-0.25f,0.0f,0.0f,
		0.0f,0.5f,0.0f,

	};



	Shader mShader("vertex.glsl", "fragment.glsl");
	Shader mShader_yellow("vertex.glsl", "fragment_yellow.glsl");
	Triangle mTriangle(vertices, sizeof(vertices)/sizeof(float));
	Triangle mTriangle2(vertices2, sizeof(vertices2) / sizeof(float));

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mShader.use();
		mTriangle.draw();

		mShader_yellow.use();
		mTriangle2.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

