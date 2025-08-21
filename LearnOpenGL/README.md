## Triangle

```cpp
	float vertices[] = {
		 0.25f, -0.5f, 0.0f, 1.0f,0.0f,1.0f,// left  
		 0.75f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,// right 
		 0.5f,  0.0f, 0.0f,  0.0f,0.0f,1.0f,// top 
	};

	// Shader作成
	Shader mShader("vertex.glsl", "fragment.glsl");

	// 図形作成
	Triangle mTriangle(vertices, sizeof(vertices)/sizeof(float));

	while(...){
		
		...

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(mShader.getShaderProgram(), "changeColor");
		mShader.use();
		mShader.setBool("useChangeColor", true);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // uniformがあるahsder
		mTriangle.draw();
	}
```

## Rectangle
```cpp
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

	//Shader作成
	Shader mShader_normal("vertex.glsl", "fragment.glsl");

	// Rectangle作成
	Rectangle mRectangle(
		vertices_rectangle,
		indices_rectangle,
		sizeof(vertices_rectangle) / sizeof(float),
		sizeof(indices_rectangle) / sizeof(int)
	);
	while(...){
		
		...
		float xOffset = -0.25;
		float yOffset = -0.25;
		mShader_normal.use();
		mShader_normal.setFloat("xOffset", xOffset); //x方向に平行移動
		mShader_normal.setFloat("yOffset", yOffset); //y方向に平行移動
		mRectangle.draw();
	}
```

## Texture
```cpp
	// Texture
	float vertices_texture[] = {
		// positions          // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices_texture[] = {
		0,1,3,
		1,2,3
	};

	std::vector<VertexAttribute> layout_texture = {
		{0,3,GL_FLOAT,GL_FALSE,(GLsizei)(5 * sizeof(float)),0,false}, //position
		{1,2,GL_FLOAT,GL_FALSE,(GLsizei)(5 * sizeof(float)),(size_t)(3 * sizeof(float)),false}  //index}
	};


	Texture mTexture(
		vertices_texture,
		sizeof(vertices_texture),
		indices_texture,
		sizeof(indices_texture),
		GL_UNSIGNED_INT,
		layout_texture
	);

	// Shader作成
	Shader mShader_texture("vertex_texture.glsl", "fragment_texture.glsl");
	
	// 画像の登録
	mTexture.initializeTexture("Assets/container.jpg", 0);
	mTexture.initializeTexture("Assets/awesome_face.jpg", 1);
	

	// uniform
	mShader_texture.use();
	mShader_texture.setInt("texture1", 0);
	mShader_texture.setInt("texture2", 1);

	// matrix
	glm::mat4 model = glm::mat4(1.0f); 
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // model matrix

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f)); // view matrix

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f); // projection matrix

	mShader_texture.setMatrix4("model", model);
	mShader_texture.setMatrix4("view", view);
	mShader_texture.setMatrix4("projection", projection);


	while(...){
		
		...
		mShader_texture.use();
		mShader_texture.setMatrix4("transform", transform1);
		mTexture.draw();
		mShader_texture.setFloat("mixValue", mixValue);

		mShader_texture.setMatrix4("transform", transform2);
		mTexture.draw();
	}
```

## Cube
```cpp
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

	// Shader作成
	Shader mShader_cube("vertex_texture.glsl", "fragment_texture.glsl");

	// Cube作成
	std::vector<VertexAttribute> layout_cube = {
		{0,3,GL_FLOAT,GL_FALSE,(GLsizei)(5 * sizeof(float)),0,false}, //position
		{1,2,GL_FLOAT,GL_FALSE,(GLsizei)(5 * sizeof(float)),(size_t)(3 * sizeof(float)),false} //index 
	};


	Texture mTexture_cube(
		vertices_cube,
		sizeof(vertices_cube),     
		indices_cube,
		sizeof(indices_cube),      
		GL_UNSIGNED_INT,
		layout_cube                
	);

	// 画像の登録
	mTexture_cube.initializeTexture("Assets/container.jpg", 0);
	mTexture_cube.initializeTexture("Assets/awesome_face.jpg", 1);

	// uniform 
	mShader_cube.use();
	mShader_cube.setInt("texture1", 0);
	mShader_cube.setInt("texture2", 1);

	while(...){

	    ...

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
			if (i % 3 != 0) {
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else {
				model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			}
			mShader_cube.setMatrix4("model", model);
			mTexture_cube.draw();
		}
	}
```

## Camera
```cpp
int main(){
    // カーソルキャプチャ
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// カメラ生成
	Camera mCamera(
		glm::vec3(0.0f, 0.0f, 3.0f), // カメラの位置
		glm::vec3(0.0f, 0.0f, -1.0f), // カメラの前方ベクトル
		glm::vec3(0.0f, 1.0f, 0.0f)   // カメラの上方向ベクトル
	);

	// callbackからmCameraにアクセスできるようにする
	glfwSetWindowUserPointer(window, &mCamera);

	while(...){
	    ...
		
		// view matrix
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(mCamera.getPosition(), mCamera.getPosition() + mCamera.getFront(), mCamera.getUp());

		// projection matrix
		projection = glm::perspective(glm::radians(mCamera.getFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		mShader_cube.use();
	}
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
```

## Basic lighting
```cpp
float vertices_cube[] = {
		// Front (+Z)
		-0.5f,-0.5f, 0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f,-0.5f, 0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f, 0.5f, 0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,  0.0f, 1.0f,

		// Back (-Z)
		-0.5f,-0.5f,-0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f,-0.5f,-0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f, 0.5f,-0.5f, 0.0f,  0.0f, -1.0f,
		-0.5f, 0.5f,-0.5f, 0.0f,  0.0f, -1.0f,

		// Left (-X)
		-0.5f,-0.5f, 0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,-0.5f,-0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, 0.5f,-0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f,  0.0f,  0.0f,
		 
		// Right (+X)
		 0.5f,-0.5f, 0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f,-0.5f,-0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f, 0.5f,-0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f, 0.5f, 0.5f, 1.0f,  0.0f,  0.0f,

		// Bottom (-Y)
		-0.5f,-0.5f,-0.5f, 0.0f, -1.0f,  0.0f,
		 0.5f,-0.5f,-0.5f, 0.0f, -1.0f,  0.0f,
		 0.5f,-0.5f, 0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f,-0.5f, 0.5f, 0.0f, -1.0f,  0.0f,

		// Top (+Y)
		-0.5f, 0.5f,-0.5f, 0.0f,  1.0f,  0.0f,
		 0.5f, 0.5f,-0.5f, 0.0f,  1.0f,  0.0f,
		 0.5f, 0.5f, 0.5f, 0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,  1.0f,  0.0f,
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

	std::vector<VertexAttribute> layout_cube = {
		{0,3,GL_FLOAT,GL_FALSE,(GLsizei)(6 * sizeof(float)),0,false},
		{1,3,GL_FLOAT,GL_FALSE,(GLsizei)(6 * sizeof(float)),(size_t)(3 * sizeof(float)),false} // normal vector
	};

	Cube mCube(
		vertices_cube,
		sizeof(vertices_cube),
		indices_cube,
		sizeof(indices_cube),
		GL_UNSIGNED_INT,
		layout_cube
	);

	// light ///////////////////////////////////////////////////////////
	glm::vec3 mLightPos(1.2f, 1.0f, 2.0f);
	std::vector<VertexAttribute> layout_light = {
		{0,3,GL_FLOAT,GL_FALSE,(GLsizei)(6 * sizeof(float)),0,false}
	};

	Cube mLight(
		vertices_cube,
		sizeof(vertices_cube),
		indices_cube,
		sizeof(indices_cube),
		GL_UNSIGNED_INT,
		layout_light
	);


	while(...){
		...
		mLightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		mLightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(mCamera.getPosition(), mCamera.getPosition() + mCamera.getFront(), mCamera.getUp());
		glm::mat4 projection = glm::perspective(glm::radians(mCamera.getFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		mShader_cube.use();
		mShader_cube.setMatrix4("view", view);
		mShader_cube.setMatrix4("projection", projection);
		mShader_cube.setVec3("objectColor", glm::vec3(1.0f, 0.4, 0.31f));
		mShader_cube.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glm::vec3 lightViewPos = glm::vec3(view * glm::vec4(mLightPos, 1.0f));
		mShader_cube.setVec3("lightViewPos", mLightPos);
		
		for (unsigned int i = 0; i < 10; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (i % 3 != 0) {
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else {
				model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			}
			mShader_cube.setMatrix4("model", model);
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * model)));

			mShader_cube.setMatrix3("normalMatrix", normalMatrix);
			mCube.draw();
		}

		//mCube.draw();

		// light////////////
		mShader_lighting.use();
		mShader_lighting.setMatrix4("projection", projection);
		mShader_lighting.setMatrix4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, mLightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		mShader_lighting.setMatrix4("model", model);

		mLight.draw();;
	}
```

## Material
```cpp
	float vertices_cube[] = {
		// Front (+Z)
		-0.5f,-0.5f, 0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f,-0.5f, 0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f, 0.5f, 0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,  0.0f, 1.0f,

		// Back (-Z)
		-0.5f,-0.5f,-0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f,-0.5f,-0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f, 0.5f,-0.5f, 0.0f,  0.0f, -1.0f,
		-0.5f, 0.5f,-0.5f, 0.0f,  0.0f, -1.0f,

		// Left (-X)
		-0.5f,-0.5f, 0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,-0.5f,-0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, 0.5f,-0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f,  0.0f,  0.0f,
		 
		// Right (+X)
		 0.5f,-0.5f, 0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f,-0.5f,-0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f, 0.5f,-0.5f, 1.0f,  0.0f,  0.0f,
		 0.5f, 0.5f, 0.5f, 1.0f,  0.0f,  0.0f,

		// Bottom (-Y)
		-0.5f,-0.5f,-0.5f, 0.0f, -1.0f,  0.0f,
		 0.5f,-0.5f,-0.5f, 0.0f, -1.0f,  0.0f,
		 0.5f,-0.5f, 0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f,-0.5f, 0.5f, 0.0f, -1.0f,  0.0f,

		// Top (+Y)
		-0.5f, 0.5f,-0.5f, 0.0f,  1.0f,  0.0f,
		 0.5f, 0.5f,-0.5f, 0.0f,  1.0f,  0.0f,
		 0.5f, 0.5f, 0.5f, 0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f,  1.0f,  0.0f,
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

	std::vector<VertexAttribute> layout_cube = {
		{0,3,GL_FLOAT,GL_FALSE,(GLsizei)(6 * sizeof(float)),0,false},
		{1,3,GL_FLOAT,GL_FALSE,(GLsizei)(6 * sizeof(float)),(size_t)(3 * sizeof(float)),false} // normal vector
	};

	Cube mCube(
		vertices_cube,
		sizeof(vertices_cube),
		indices_cube,
		sizeof(indices_cube),
		GL_UNSIGNED_INT,
		layout_cube
	);

	// light ///////////////////////////////////////////////////////////
	glm::vec3 mLightPos(1.2f, 1.0f, 2.0f);
	glm::vec3 mLightAmbient(0.2f, 0.2f, 0.2f);
	glm::vec3 mLightDiffuse(0.5f, 0.5f, 0.5f);
	glm::vec3 mLightSpecular(1.0f, 1.0f, 1.0f);
	std::vector<VertexAttribute> layout_light = {
		{0,3,GL_FLOAT,GL_FALSE,(GLsizei)(6 * sizeof(float)),0,false}
	};

	Cube mLight(
		vertices_cube,
		sizeof(vertices_cube),
		indices_cube,
		sizeof(indices_cube),
		GL_UNSIGNED_INT,
		layout_light
	);

	while(...){
		mLightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		mLightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(mCamera.getPosition(), mCamera.getPosition() + mCamera.getFront(), mCamera.getUp());
		glm::mat4 projection = glm::perspective(glm::radians(mCamera.getFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		mShader_cube.use();
		mShader_cube.setMatrix4("view", view);
		mShader_cube.setMatrix4("projection", projection);
		// material
		mShader_cube.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		mShader_cube.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		mShader_cube.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		mShader_cube.setFloat("material.shininess", 32.0f);

		// light
		glm::vec3 lightViewPos = glm::vec3(view * glm::vec4(mLightPos, 1.0f));
		mShader_cube.setVec3("light.viewPosition", lightViewPos);
		mShader_cube.setVec3("light.ambient", mLightAmbient);
		mShader_cube.setVec3("light.diffuse", mLightDiffuse);
		mShader_cube.setVec3("light.specular", mLightSpecular);

		for (unsigned int i = 0; i < 10; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (i % 3 != 0) {
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else {
				model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			}
			mShader_cube.setMatrix4("model", model);
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * model)));

			mShader_cube.setMatrix3("normalMatrix", normalMatrix);
			mCube.draw();
		}

		//mCube.draw();

		// light////////////
		mShader_lighting.use();
		mShader_lighting.setMatrix4("projection", projection);
		mShader_lighting.setMatrix4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, mLightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		mShader_lighting.setMatrix4("model", model);

		mLight.draw();
	}
```

## Diffuse Map

```cpp
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

	std::vector<VertexAttribute> layout_cube = {
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
	mShader_cube.setInt("material.specular", 1);

	while(...){
		...
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(mCamera.getPosition(), mCamera.getPosition() + mCamera.getFront(), mCamera.getUp());
		glm::mat4 projection = glm::perspective(glm::radians(mCamera.getFov()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		mShader_cube.use();
		mShader_cube.setMatrix4("view", view);
		mShader_cube.setMatrix4("projection", projection);
		// material
		mShader_cube.setFloat("material.shininess", 32.0f);

		// light
		glm::vec3 lightViewPos = glm::vec3(view * glm::vec4(mLightPos, 1.0f));
		mShader_cube.setVec3("light.viewPosition", lightViewPos);
		mShader_cube.setVec3("light.ambient", mLightAmbient);
		mShader_cube.setVec3("light.diffuse", mLightDiffuse);
		mShader_cube.setVec3("light.specular", mLightSpecular);

		for (unsigned int i = 0; i < 10; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (i % 3 != 0) {
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else {
				model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			}
			mShader_cube.setMatrix4("model", model);
			glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(view * model)));

			mShader_cube.setMatrix3("normalMatrix", normalMatrix);
			mCube.draw();
		}

		//mCube.draw();

		// light////////////
		mShader_lighting.use();
		mShader_lighting.setMatrix4("projection", projection);
		mShader_lighting.setMatrix4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, mLightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		mShader_lighting.setMatrix4("model", model);

		mLight.draw();
	}
```