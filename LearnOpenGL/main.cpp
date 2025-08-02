#define STB_IMAGE_IMPLEMENTATION

#include"std_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "Common.h"
#include "rectangle.h"
#include "triangle.h"
#include <filesystem>



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
		 0.25f, -0.5f, 0.0f, 1.0f,0.0f,1.0f,// left  
		 0.75f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,// right 
		 0.5f,  0.0f, 0.0f,  0.0f,0.0f,1.0f,// top 
	};

	float vertices_rectangle[] = {
		// position            // color
		-0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,  // 0: left bottom
		-0.5f,  0.5f, 0.0f,    0.0f, 1.0f, 0.0f,  // 1: left top
		 0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  // 2: right bottom
		 0.5f,  0.5f, 0.0f,    0.0f, 0.0f, 0.0f   // 3: right top
	};
	unsigned int indices_rectangle[] = {
		0, 1, 2,
		2, 1, 3
	};



	// Shader作成
	Shader mShader("vertex.glsl", "fragment.glsl");
	Shader mShader_normal("vertex.glsl", "fragment.glsl");
	Shader mShader_texture("vertex_texture.glsl", "fragment_texture.glsl");

	// 図形作成
	Triangle mTriangle(vertices, sizeof(vertices)/sizeof(float));
	Rectangle mRectangle(
		vertices_rectangle,
		indices_rectangle,
		sizeof(vertices_rectangle) / sizeof(float),
		sizeof(indices_rectangle) / sizeof(int)
	);



	// Texture
	float vertices_texture[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices_texture[] = {
		0,1,3,
		1,2,3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_texture), vertices_texture, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_texture), indices_texture, GL_STATIC_DRAW);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(5);

	unsigned int texture;
	glGenTextures(1, &texture); // 生成されたIDをtexutreに格納
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load Texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Assets/container.jpg", &width, &height, &nrChannels, 0);

	// Texture作成
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);


	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Triangle
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(mShader.getShaderProgram(), "changeColor");
		mShader.use();
		mShader.setBool("useChangeColor", true);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // uniformがあるahsder
		mTriangle.draw();

		// Rectangle
		float xOffset = -0.25;
		float yOffset = -0.25;
		mShader_normal.use();
		mShader_normal.setFloat("xOffset", xOffset); //x方向に平行移動
		mShader_normal.setFloat("yOffset", yOffset); //y方向に平行移動
		mRectangle.draw();

		// Texture
		mShader_texture.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

