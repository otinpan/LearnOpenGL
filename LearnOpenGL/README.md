# OpenGL
## Vertex input
**VBO** (Vertex Buffer Object) は頂点データをGPUのメモリに格
納するためのオブジェクトで、1種類のデータに対して1つのVBOを作成する。

```cpp
//vertex
	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
	};



	// VBO (Vertex Buffer Object) /////////////////////////////////////////////////////////////////////////////////
	unsigned int VBO;
	glGenBuffers(1, &VBO); //VBOを作成
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //頂点の位置情報とVBOをバインド;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //verticesをGPUにアップロード
```

* `glBindBuffer()`：これから行う頂点データへの処理はVBOに対するものと宣言
* `glBufferData()`：頂点データをGPUに送信する


## Vertex shader
GPUが描画データを受け取った後、最初に実行されるのが**Vertex shader**。OpenGL専用のGLSLという言語で
書かれる。Vertex shaderの役割は、各頂点の座標や色を計算すること。

```cpp
	// Vertex shader ///////////////////////////////////////////////////////////////////////
	const char* vertexShaderSource = "#version 330 core\n"  //GLSLで書かれた計算処理
		"layout (location=0)in vec3 aPos;\n"
		"void main(){\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Shader作成
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Shaderとソースコードを対応
	glCompileShader(vertexShader); //コンパイル

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
```

* `vertexShaderSource`：GLSLで書かれたコード。ここでは`location=0`のデータに対して処理を行おうとしている。
* `glCompileShader()：vertexShader`をコンパイル

## Fragment shader
スクリーンの各画素について実行される。それぞれのピクセルとそれらをなんの色
で塗るかを計算する。

```cpp
	// Fragment shader //////////////////////////////////////////////////////////
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main(){\n"
		"	FragColor=vec4(1.0f,0.5f,0.2f,1.0f);\n"
		"}\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
```

* `out`：この変数は出力であることを指定する
* `FragColor`：出力となる変数名

## Linking Vertex Attributes
いまの状態は
* GPUに頂点データを送信s
* Vertex shaderをコンパイル
* Fragment shaderをコンパイル

このままでは何も表示されない。またシェーダーとVBOはなにも関連していない。
haderとVBOを関連付ける必要がある。

```cpp
// Layout (loction=0)としてシェーダーに渡す
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //Layoutの変数にVBOを紐づけ
glEnableVertexAttribArray(0); //VBOの中身をシェーダーに送る
```


ただ、もし複数のVBOがあり、それに対してシェーダーで処理したいとき、何回も
`glVertexAttribPointer`でVBOとshaderを指定する必要が出てくる。

```cpp

// ---------- 位置情報のVBO ----------
unsigned int VBO_position;
glGenBuffers(1, &VBO_position);
glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

// layout (location = 0) としてシェーダに渡す
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// ---------- 色情報のVBO ----------
unsigned int VBO_color;
glGenBuffers(1, &VBO_color);
glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

// layout (location = 1) としてシェーダに渡す
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(1);
```
また`glVertexAttribPointer()`の設定は毎フレーム失われるため、毎回ループで
設定する必要がある。

```cpp
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, VBO); //現在使うVBOを指定
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //Layoutの変数にVBOを紐づけ
		glEnableVertexAttribArray(0); //VBOの中身をシェーダーに送る
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();  //イベントがあればcallbackを呼ぶ
	}
```

<details>

```cpp
// glad.h は最初に！
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



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


	//vertex
	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
	};



	// VBO (Vertex Buffer Object) /////////////////////////////////////////////////////////////////////////////////
	unsigned int VBO;
	glGenBuffers(1, &VBO); //VBOを作成
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //頂点の位置情報とVBOをバインド;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //verticesをGPUにアップロード


	// 頂点属性の設定
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //Layoutの変数にVBOを紐づけ
	glEnableVertexAttribArray(0); //VBOの中身をシェーダーに送る

	// Vertex shader ///////////////////////////////////////////////////////////////////////
	const char* vertexShaderSource = "#version 330 core\n"  //GLSLで書かれた計算処理
		"layout (location=0)in vec3 aPos;\n"
		"void main(){\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Shader作成
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Shaderとソースコードを対応
	glCompileShader(vertexShader); //コンパイル

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader //////////////////////////////////////////////////////////
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main(){\n"
		"	FragColor=vec4(1.0f,0.5f,0.2f,1.0f);\n"
		"}\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // shader program作成
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); // vertex shaderとfragment shaderをアタッチ (ラスタライズは勝手にやってくれる) 
	glLinkProgram(shaderProgram); // vertex shaderの出力とfragment shaderの入力が正しくつながっていないとエラー

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(shaderProgram); 
	glDeleteShader(vertexShader); 
	glDeleteShader(fragmentShader);



	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, VBO); //現在使うVBOを指定
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //Layoutの変数にVBOを紐づけ
		glEnableVertexAttribArray(0); //VBOの中身をシェーダーに送る
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();  //イベントがあればcallbackを呼ぶ
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
}
```

</details>

### VAO
**VAO** (Vertex Array Object) はループのたびにで頂点とシェーダーを結びつける必要がなくなる。
具体的には以下の情報を保持する
* `glEnableVertexAttribArray`の呼び出し状態
* `glVertexAttibPointer`によって設定された頂点属性
* `glVertexAttribPointer`を通じて頂点属性に関連付けられたVBO

<details>

```cpp
// glad.h は最初に！
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



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


	//vertex
	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
	};



	// VBO (Vertex Buffer Object)  VAO (Vertex Array Object) ////////////////////////////////////////
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBOのバインドを解除
	glBindVertexArray(0); //VAOのバインドを解除

	// Vertex shader ///////////////////////////////////////////////////////////////////////
	const char* vertexShaderSource = "#version 330 core\n"  //GLSLで書かれた計算処理
		"layout (location=0)in vec3 aPos;\n"
		"void main(){\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Shader作成
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Shaderとソースコードを対応
	glCompileShader(vertexShader); //コンパイル

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader //////////////////////////////////////////////////////////
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main(){\n"
		"	FragColor=vec4(1.0f,0.5f,0.2f,1.0f);\n"
		"}\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // shader program作成
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); // vertex shaderとfragment shaderをアタッチ (ラスタライズは勝手にやってくれる) 
	glLinkProgram(shaderProgram); // vertex shaderの出力とfragment shaderの入力が正しくつながっていないとエラー

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(shaderProgram); 
	glDeleteShader(vertexShader); 
	glDeleteShader(fragmentShader);



	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();  //イベントがあればcallbackを呼ぶ
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
}
```

</details>

### 四角形の描画


<details>

```cpp
// glad.h は最初に！
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



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


	//vertex
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f // top left
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	// VBO VAO EBO (Elemental Buffer Object)  //////////////////////////////////////////////////
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBOのバインドを解除
	glBindVertexArray(0); //VAOのバインドを解除

	// Vertex shader ///////////////////////////////////////////////////////////////////////
	const char* vertexShaderSource = "#version 330 core\n"  //GLSLで書かれた計算処理
		"layout (location=0)in vec3 aPos;\n"
		"void main(){\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Shader作成
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Shaderとソースコードを対応
	glCompileShader(vertexShader); //コンパイル

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader //////////////////////////////////////////////////////////
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main(){\n"
		"	FragColor=vec4(1.0f,0.5f,0.2f,1.0f);\n"
		"}\0";

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // shader program作成
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); // vertex shaderとfragment shaderをアタッチ (ラスタライズは勝手にやってくれる) 
	glLinkProgram(shaderProgram); // vertex shaderの出力とfragment shaderの入力が正しくつながっていないとエラー

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();  //イベントがあればcallbackを呼ぶ
	}


	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

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
}
```
</details>

