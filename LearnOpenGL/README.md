## Triangle

```cpp
	float vertices[] = {
		 0.25f, -0.5f, 0.0f, 1.0f,0.0f,1.0f,// left  
		 0.75f, -0.5f, 0.0f, 0.0f,1.0f,0.0f,// right 
		 0.5f,  0.0f, 0.0f,  0.0f,0.0f,1.0f,// top 
	};

	// ShaderçÏê¨
	Shader mShader("vertex.glsl", "fragment.glsl");

	// ê}å`çÏê¨
	Triangle mTriangle(vertices, sizeof(vertices)/sizeof(float));

	while(...){
		
		...

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(mShader.getShaderProgram(), "changeColor");
		mShader.use();
		mShader.setBool("useChangeColor", true);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // uniformÇ™Ç†ÇÈahsder
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

	//ShaderçÏê¨
	Shader mShader_normal("vertex.glsl", "fragment.glsl");

	// RectangleçÏê¨
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
		mShader_normal.setFloat("xOffset", xOffset); //xï˚å¸Ç…ïΩçsà⁄ìÆ
		mShader_normal.setFloat("yOffset", yOffset); //yï˚å¸Ç…ïΩçsà⁄ìÆ
		mRectangle.draw();
	}
```

## Texture
```cpp
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

	// ShaderçÏê¨
	Shader mShader_texture("vertex_texture.glsl", "fragment_texture.glsl");

	// TextureçÏê¨
	Texture mTexture(
		vertices_texture,
		indices_texture,
		sizeof(vertices_texture) / sizeof(float),
		sizeof(indices_texture) / sizeof(int)
	);
	
	// âÊëúÇÃìoò^
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
		// ëOñ 
		0, 1, 2,  2, 3, 0,
		// îwñ 
		4, 5, 6,  6, 7, 4,
		// ç∂ë§ñ 
		8, 9,10, 10,11, 8,
		// âEë§ñ 
	    12,13,14, 14,15,12,
	    // íÍñ 
	    16,17,18, 18,19,16,
	    // è„ñ 
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

	// ShaderçÏê¨
	Shader mShader_cube("vertex_texture.glsl", "fragment_texture.glsl");

	// CubeçÏê¨
	Texture mTexture_cube(
		vertices_cube,
		indices_cube,
		sizeof(vertices_cube) / sizeof(float),
		sizeof(indices_cube)/sizeof(int)
	);

	// âÊëúÇÃìoò^
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