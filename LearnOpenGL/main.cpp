#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>

#include "shader.h"
#include "rectangle.h"
#include "triangle.h"
#include "texture.h"
#include "camera.h"
#include "vertex_layout.h" 
#include "cube.h"
#include "model.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

float mixValue = 0.2f;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLADの初期化
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// カーソルキャプチャ
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 深度テスト有効化
	glEnable(GL_DEPTH_TEST);

	// カメラ生成
	Camera mCamera(
		glm::vec3(0.0f, 0.0f, 3.0f), // カメラの位置
		glm::vec3(0.0f, 0.0f, -1.0f), // カメラの前方ベクトル
		glm::vec3(0.0f, 1.0f, 0.0f)   // カメラの上方向ベクトル
	);

	// callbackからmCameraにアクセスできるようにする
	glfwSetWindowUserPointer(window, &mCamera);


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
	Shader mShader_cube("vertex_combined_light.glsl", "fragment_combined_light.glsl");
	Shader mShader_lighting("vertex_light_cube.glsl", "fragment_light_cube.glsl");
	Shader mShader_model("vertex_model.glsl", "fragment_model.glsl");

	// 図形作成
	Triangle mTriangle(vertices, sizeof(vertices) / sizeof(float));
	Rectangle mRectangle(
		vertices_rectangle,
		indices_rectangle,
		sizeof(vertices_rectangle) / sizeof(float),
		sizeof(indices_rectangle) / sizeof(int)
	);

	// Cube
	float vertices_cube[] = {
		// Front (+Z)
		-0.5f,-0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,0.0f,
		 0.5f,-0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 1.0f,0.0f,
		 0.5f, 0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 1.0f,1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,1.0f,

		// Back (-Z)
		-0.5f,-0.5f,-0.5f, 0.0f,  0.0f, -1.0f, 0.0f,0.0f,
		 0.5f,-0.5f,-0.5f, 0.0f,  0.0f, -1.0f, 1.0f,0.0f,
		 0.5f, 0.5f,-0.5f, 0.0f,  0.0f, -1.0f, 1.0f,1.0f,
		-0.5f, 0.5f,-0.5f, 0.0f,  0.0f, -1.0f, 0.0f,1.0f,

		// Left (-X)
		-0.5f,-0.5f, 0.5f, -1.0f,  0.0f,  0.0f, 0.0f,0.0f,
		-0.5f,-0.5f,-0.5f, -1.0f,  0.0f,  0.0f, 1.0f,0.0f,
		-0.5f, 0.5f,-0.5f, -1.0f,  0.0f,  0.0f, 1.0f,1.0f,
		-0.5f, 0.5f, 0.5f, -1.0f,  0.0f,  0.0f, 0.0f,1.0f,
		 
		// Right (+X)
		 0.5f,-0.5f, 0.5f, 1.0f,  0.0f,  0.0f, 0.0f,0.0f,
		 0.5f,-0.5f,-0.5f, 1.0f,  0.0f,  0.0f, 1.0f,0.0f,
		 0.5f, 0.5f,-0.5f, 1.0f,  0.0f,  0.0f, 1.0f,1.0f,
		 0.5f, 0.5f, 0.5f, 1.0f,  0.0f,  0.0f, 0.0f,1.0f,

		// Bottom (-Y)
		-0.5f,-0.5f,-0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,-0.5f,-0.5f, 0.0f, -1.0f,  0.0f, 1.0f,0.0f,
		 0.5f,-0.5f, 0.5f, 0.0f, -1.0f,  0.0f, 1.0f,1.0f,
		-0.5f,-0.5f, 0.5f, 0.0f, -1.0f,  0.0f, 0.0f,1.0f,

		// Top (+Y)
		-0.5f, 0.5f,-0.5f, 0.0f,  1.0f,  0.0f, 0.0f,0.0f,
		 0.5f, 0.5f,-0.5f, 0.0f,  1.0f,  0.0f, 1.0f,0.0f,
		 0.5f, 0.5f, 0.5f, 0.0f,  1.0f,  0.0f, 1.0f,1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,  1.0f,  0.0f, 0.0f,1.0f,
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

	/*std::vector<VertexAttribute> layout_cube = {
		{0,3,GL_FLOAT,GL_FALSE,(GLsizei)(8 * sizeof(float)),0,false},
		{1,3,GL_FLOAT,GL_FALSE,(GLsizei)(8 * sizeof(float)),(size_t)(3 * sizeof(float)),false}, // normal vector
		{2,2,GL_FLOAT,GL_FALSE,(GLsizei)(8 * sizeof(float)),(size_t)(6 * sizeof(float)),false} // texture coordinates
	};

	Texture mCube(
		vertices_cube,
		sizeof(vertices_cube),
		indices_cube,
		sizeof(indices_cube),
		GL_UNSIGNED_INT,
		layout_cube
	);
	mCube.initializeTexture("Assets/container2.png", 0);
	mCube.initializeTexture("Assets/container2_specular.png", 1);

	mShader_cube.use();
	mShader_cube.setInt("material.diffuse", 0);
	mShader_cube.setInt("material.specular", 1);*/


	// light ///////////////////////////////////////////////////////////
	glm::vec3 mLightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 mLightAmbient(0.8f, 0.8f, 0.8f);
	glm::vec3 mLightDiffuse(0.8f, 0.8f, 0.8f);
	glm::vec3 mLightSpecular(1.0f, 1.0f, 1.0f);
	std::vector<VertexAttribute> layout_light = {
		{0,3,GL_FLOAT,GL_FALSE,(GLsizei)(8 * sizeof(float)),0,false}
	};

	Cube mLight(
		vertices_cube,
		sizeof(vertices_cube),
		indices_cube,
		sizeof(indices_cube),
		GL_UNSIGNED_INT,
		layout_light
	);

	// model ///////////////////////////////////////////////////////////
	Model mModel("Assets/objects/backpack/backpack.obj");



	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 view = glm::lookAt(mCamera.getPosition(), mCamera.getPosition() + mCamera.getFront(), mCamera.getUp());
		glm::mat4 projection = glm::perspective(glm::radians(mCamera.getFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// Shader model
		mShader_model.use();
		mShader_model.setMatrix4("view", view);
		mShader_model.setMatrix4("projection", projection);
		mShader_model.setMatrix4("model", model);
		//glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
		//mShader_model.setMatrix3("normalMatrix", normalMatrix);

		// Directional light
		/*mShader_model.setVec3("dirLight.direction", glm::vec3(1.0f, 0.3f, 0.5f));
		mShader_model.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		mShader_model.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		mShader_model.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		// Point light
		mShader_model.setVec3("pointLight.position", mLightPos);
		mShader_model.setVec3("pointLight.ambient", mLightAmbient);
		mShader_model.setVec3("pointLight.diffuse", mLightDiffuse);
		mShader_model.setVec3("pointLight.specular", mLightSpecular);
		mShader_model.setFloat("pointLight.constant", 1.0f);
		mShader_model.setFloat("pointLight.linear", 0.09f);
		mShader_model.setFloat("pointLight.quadratic", 0.032f);

		// spotLight
		mShader_model.setVec3("spotlight.position", mCamera.getPosition());
		mShader_model.setVec3("spotlight.direction", mCamera.getFront());
		mShader_model.setFloat("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
		mShader_model.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(15.0f)));
		mShader_model.setVec3("spotlight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		mShader_model.setVec3("spotlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		mShader_model.setVec3("spotlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		mShader_model.setFloat("spotlight.constant", 1.0f);
		mShader_model.setFloat("spotlight.linear", 0.09f);
		mShader_model.setFloat("spotlight.quadratic", 0.032f);*/

		// light////////////
		mShader_lighting.use();
		mShader_lighting.setMatrix4("projection", projection);
		mShader_lighting.setMatrix4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, mLightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		mShader_lighting.setMatrix4("model", model);

		mLight.draw();

		mShader_model.use();
		mModel.draw(mShader_model);
		

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (!cam)return;
	cam->updateDirection(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (!cam)return;
	cam->updateFov(yoffset);
}

void processInput(GLFWwindow* window) {
	// カメラの移動
	auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (!cam)return;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam->updatePosition(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam->updatePosition(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam->updatePosition(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam->updatePosition(RIGHT, deltaTime);
	}


	// windowサイズ
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}


	// mixValueの調整
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