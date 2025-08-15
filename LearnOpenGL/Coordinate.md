# Coodinate Systems

���W�͎��5�̌n�ɕ�������
* Local space
* World space
* View space
* Clip space
* Screen space

![](https://learnopengl.com/img/getting-started/coordinate_systems.png)

## Local space
�I�u�W�F�N�g�Ǝ��̍��W�n�B�Ⴆ�΁A1�̔��͂��ꂼ��(0,0,0)�̍��W��
�����Ƃ�����B

## World space
�I�u�W�F�N�g�����݂��鐢�E�̍��W�n�BModel matrix��p����Local space����World space
�֕ϊ������Bscale�Arotate�Atranslate�̕ϊ����s����B

## View space
�v���C���[�̖ڐ����猩�����W�n�BView matrix��p����Worlad space
����View space�֕ϊ������B�J�����̈ʒu��������ς���View space���ς��B

## Clip space
���͈͔�����s�����߂̍��W�n�BPorjection matrix��p����
View space����Clip space�ɕϊ������B�����A�ݒ肵��**frustum***
( ������ )�͈̔͂ɓ����Ă��Ȃ���΁A�`�悳��Ȃ��Bfrustum��**Orthographic projection**��
**Perspective projection** ��2��ނ�����B

### Orthographic projection
Orthographic projection�ł͉����̕��̂��߂��̕��̂������吓�ŕ`�悳���B
![](https://learnopengl.com/img/getting-started/orthographic_frustum.png)

```cpp
glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
```
��1�����͍��[��x���W�A��2�����͉E�[��x���W�A��3�����͉��[��y���W�A��4�����͏�[��y���W�A
��5�����͋߂���z���W�A��6�����͉�����z���W���w�肷��B

### Prespective projection
Prespective projection�ł͉����̕��̂͏������A�߂��̕��̂͑傫���`�悳���B��茻���ɋ߂��B
`w`���W���e���W�Ɋ��邱�ƂŁA���ߖ@��\������B
![](https://learnopengl.com/img/getting-started/perspective_frustum.png)
NEAR PLANE��FAR PLANE�̊Ԃɂ��镨�̂��`�悳���B

```cpp
glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
```
��1�����͎���p (FOV)�A��2�����̓A�X�y�N�g�� (width/height)�A��3�����͋߂���z���W�A��4�����͉�����z���W���w�肷��B

����NEAR PLANE�̒l��傫����������ƁA�v���C���[�͉����̕��̂����悤�Ƃ��邽�߁A�I�u�W�F�N�g�̓�����
�������肷�邱�Ƃ�����B�����A���_��[-w,w]�͈̔͂Ɏ��܂��Ă��Ȃ��ꍇ�A�����frustum�̊O�ɂ���Ƃ���
�󋵂ɂȂ�B

![](https://learnopengl.com/img/getting-started/perspective_orthographic.png)

## 3D
`uniform`��p����shader�ɕϊ��s���n���B
```glsl
#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position =projection*view*model*vec4(aPos,1.0);
	TexCoord=aTexCoord;
}
```
```cpp
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		
```

�������̂̐}�`��`�悵�����ꍇ�́Avertex�̍��W�𑝂₷�K�v������B

```cpp
	// Cube
	float vertices_cube[] = {
		// Front (+Z)
	    -0.5f,-0.5f, 0.5f, 0.0f,0.0f, // 0
	     0.5f,-0.5f, 0.5f, 1.0f,0.0f, // 1
	     0.5f, 0.5f, 0.5f, 1.0f,1.0f, // 2
	    -0.5f, 0.5f, 0.5f, 0.0f,1.0f, // 3
	
	    // Back (-Z)
	    -0.5f,-0.5f,-0.5f, 0.0f,0.0f, // 4
	     0.5f,-0.5f,-0.5f, 1.0f,0.0f, // 5
	     0.5f, 0.5f,-0.5f, 1.0f,1.0f, // 6
	    -0.5f, 0.5f,-0.5f, 0.0f,1.0f, // 7
	
	    // Left (-X)
	    -0.5f,-0.5f, 0.5f, 0.0f,0.0f, // 8
	    -0.5f,-0.5f,-0.5f, 0.0f,1.0f, // 9
	    -0.5f, 0.5f,-0.5f, 1.0f,1.0f, // 10
	    -0.5f, 0.5f, 0.5f, 1.0f,0.0f, // 11
	
	    // Right (+X)
	     0.5f,-0.5f, 0.5f, 0.0f,0.0f, // 12
	     0.5f,-0.5f,-0.5f, 0.0f,1.0f, // 13
	     0.5f, 0.5f,-0.5f, 1.0f,1.0f, // 14
	     0.5f, 0.5f, 0.5f, 1.0f,0.0f, // 15
	
	    // Bottom (-Y)
	    -0.5f,-0.5f,-0.5f, 0.0f,1.0f, // 16
	     0.5f,-0.5f,-0.5f, 1.0f,1.0f, // 17
	     0.5f,-0.5f, 0.5f, 1.0f,0.0f, // 18
	    -0.5f,-0.5f, 0.5f, 0.0f,0.0f, // 19
	
	    // Top (+Y)
	    -0.5f, 0.5f,-0.5f, 0.0f,1.0f, // 20
	     0.5f, 0.5f,-0.5f, 1.0f,1.0f, // 21
	     0.5f, 0.5f, 0.5f, 1.0f,0.0f, // 22
	    -0.5f, 0.5f, 0.5f, 0.0f,0.0f  // 23
	};

	unsigned int indices_cube[] = {
		// �O��
		0, 1, 2,  2, 3, 0,
		// �w��
		4, 5, 6,  6, 7, 4,
		// ������
		8, 9,10, 10,11, 8,
		// �E����
	    12,13,14, 14,15,12,
	    // ���
	    16,17,18, 18,19,16,
	    // ���
	    20,21,22, 22,23,20
	}; 

```

�����ł͏d��������W���폜���A`indices_cube`��p���ĕ\�����Ă���
���������̗��̂�`�悵�����ꍇ�́A`model`�s���p���č��W��ϊ�����B
```cpp
#define STB_IMAGE_IMPLEMENTATION

#include"std_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "rectangle.h"
#include "triangle.h"
#include <filesystem>
#include "texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float mixValue = 0.2f;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

	// depth
	glEnable(GL_DEPTH_TEST);

	// Cube
	float vertices_cube[] = {
		// Front (+Z)
	    -0.5f,-0.5f, 0.5f, 0.0f,0.0f, // 0
	     0.5f,-0.5f, 0.5f, 1.0f,0.0f, // 1
	     0.5f, 0.5f, 0.5f, 1.0f,1.0f, // 2
	    -0.5f, 0.5f, 0.5f, 0.0f,1.0f, // 3
	
	    // Back (-Z)
	    -0.5f,-0.5f,-0.5f, 0.0f,0.0f, // 4
	     0.5f,-0.5f,-0.5f, 1.0f,0.0f, // 5
	     0.5f, 0.5f,-0.5f, 1.0f,1.0f, // 6
	    -0.5f, 0.5f,-0.5f, 0.0f,1.0f, // 7
	
	    // Left (-X)
	    -0.5f,-0.5f, 0.5f, 0.0f,0.0f, // 8
	    -0.5f,-0.5f,-0.5f, 0.0f,1.0f, // 9
	    -0.5f, 0.5f,-0.5f, 1.0f,1.0f, // 10
	    -0.5f, 0.5f, 0.5f, 1.0f,0.0f, // 11
	
	    // Right (+X)
	     0.5f,-0.5f, 0.5f, 0.0f,0.0f, // 12
	     0.5f,-0.5f,-0.5f, 0.0f,1.0f, // 13
	     0.5f, 0.5f,-0.5f, 1.0f,1.0f, // 14
	     0.5f, 0.5f, 0.5f, 1.0f,0.0f, // 15
	
	    // Bottom (-Y)
	    -0.5f,-0.5f,-0.5f, 0.0f,1.0f, // 16
	     0.5f,-0.5f,-0.5f, 1.0f,1.0f, // 17
	     0.5f,-0.5f, 0.5f, 1.0f,0.0f, // 18
	    -0.5f,-0.5f, 0.5f, 0.0f,0.0f, // 19
	
	    // Top (+Y)
	    -0.5f, 0.5f,-0.5f, 0.0f,1.0f, // 20
	     0.5f, 0.5f,-0.5f, 1.0f,1.0f, // 21
	     0.5f, 0.5f, 0.5f, 1.0f,0.0f, // 22
	    -0.5f, 0.5f, 0.5f, 0.0f,0.0f  // 23
	};

	unsigned int indices_cube[] = {
		// �O��
		0, 1, 2,  2, 3, 0,
		// �w��
		4, 5, 6,  6, 7, 4,
		// ������
		8, 9,10, 10,11, 8,
		// �E����
	    12,13,14, 14,15,12,
	    // ���
	    16,17,18, 18,19,16,
	    // ���
	    20,21,22, 22,23,20
	}; 

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};



	Texture mTexture_cube(
		vertices_cube,
		indices_cube,
		sizeof(vertices_cube) / sizeof(float),
		sizeof(indices_cube)/sizeof(int)
	);

	mTexture_cube.initializeTexture("Assets/container.jpg", 0);
	mTexture_cube.initializeTexture("Assets/awesome_face.jpg", 1);

	// uniform 
	mShader_cube.use();
	mShader_cube.setInt("texture1", 0);
	mShader_cube.setInt("texture2", 1);


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!


		// create transformations
		//glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		mShader_cube.use();

		mShader_cube.setMatrix4("view", view);
		mShader_cube.setMatrix4("projection", projection);
		mShader_cube.setFloat("mixValue", mixValue);

		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			mShader_cube.setMatrix4("model", model);
			mTexture_cube.draw();
		}



		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixValue += 0.001f;
		if (mixValue >= 1.0f) {
			mixValue = 1.0f;
		}
	}
	if (glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixValue -= 0.001f;
		if (mixValue <= 0.0f) {
			mixValue = 0.0f;
		}
	}
}
```