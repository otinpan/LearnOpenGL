# Coodinate Systems

座標は主に5つの系に分けられる
* Local space
* World space
* View space
* Clip space
* Screen space

![](https://learnopengl.com/img/getting-started/coordinate_systems.png)

## Local space
オブジェクト独自の座標系。例えば、1つの箱はそれぞれ(0,0,0)の座標を
持つことがある。

## World space
オブジェクトが存在する世界の座標系。Model matrixを用いてLocal spaceからWorld space
へ変換される。scale、rotate、translateの変換が行われる。

## View space
プレイヤーの目線から見た座標系。View matrixを用いてWorlad space
からView spaceへ変換される。カメラの位置や向きが変わるとView spaceも変わる。

## Clip space
可視範囲判定を行うための座標系。Porjection matrixを用いて
View spaceからClip spaceに変換される。もし、設定した**frustum***
( 視錐台 )の範囲に入っていなければ、描画されない。frustumは**Orthographic projection**と
**Perspective projection** の2種類がある。

### Orthographic projection
Orthographic projectionでは遠くの物体も近くの物体も同じ大嵩で描画される。
![](https://learnopengl.com/img/getting-started/orthographic_frustum.png)

```cpp
glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
```
第1引数は左端のx座標、第2引数は右端のx座標、第3引数は下端のy座標、第4引数は上端のy座標、
第5引数は近くのz座標、第6引数は遠くのz座標を指定する。

### Prespective projection
Prespective projectionでは遠くの物体は小さく、近くの物体は大きく描画される。より現実に近い。
`w`座標を各座標に割ることで、遠近法を表現する。
![](https://learnopengl.com/img/getting-started/perspective_frustum.png)
NEAR PLANEとFAR PLANEの間にある物体が描画される。

```cpp
glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
```
第1引数は視野角 (FOV)、第2引数はアスペクト比 (width/height)、第3引数は近くのz座標、第4引数は遠くのz座標を指定する。

もしNEAR PLANEの値を大きくしすぎると、プレイヤーは遠くの物体を見ようとするため、オブジェクトの内部が
見えたりすることがある。もし、頂点が[-w,w]の範囲に収まっていない場合、それはfrustumの外にあるという
状況になる。

![](https://learnopengl.com/img/getting-started/perspective_orthographic.png)

## 3D
`uniform`を用いてshaderに変換行列を渡す。
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

もし立体の図形を描画したい場合は、vertexの座標を増やす必要がある。

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
		// 前面
		0, 1, 2,  2, 3, 0,
		// 背面
		4, 5, 6,  6, 7, 4,
		// 左側面
		8, 9,10, 10,11, 8,
		// 右側面
	    12,13,14, 14,15,12,
	    // 底面
	    16,17,18, 18,19,16,
	    // 上面
	    20,21,22, 22,23,20
	}; 

```

ここでは重複する座標を削除し、`indices_cube`を用いて表現している
もし複数の立体を描画したい場合は、`model`行列を用いて座標を変換する。
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
		// 前面
		0, 1, 2,  2, 3, 0,
		// 背面
		4, 5, 6,  6, 7, 4,
		// 左側面
		8, 9,10, 10,11, 8,
		// 右側面
	    12,13,14, 14,15,12,
	    // 底面
	    16,17,18, 18,19,16,
	    // 上面
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