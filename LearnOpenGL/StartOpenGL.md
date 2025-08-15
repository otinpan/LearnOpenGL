# OpenGL
## Vertex input
**VBO** (Vertex Buffer Object) �͒��_�f�[�^��GPU�̃������Ɋi
�[���邽�߂̃I�u�W�F�N�g�ŁA1��ނ̃f�[�^�ɑ΂���1��VBO���쐬����B

```cpp
//vertex
	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
	};



	// VBO (Vertex Buffer Object) /////////////////////////////////////////////////////////////////////////////////
	unsigned int VBO;
	glGenBuffers(1, &VBO); //VBO���쐬
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //���_�̈ʒu����VBO���o�C���h;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //vertices��GPU�ɃA�b�v���[�h
```

* `glBindBuffer()`�F���ꂩ��s�����_�f�[�^�ւ̏�����VBO�ɑ΂�����̂Ɛ錾
* `glBufferData()`�F���_�f�[�^��GPU�ɑ��M����


## Vertex shader
GPU���`��f�[�^���󂯎������A�ŏ��Ɏ��s�����̂�**Vertex shader**�BOpenGL��p��GLSL�Ƃ��������
�������BVertex shader�̖����́A�e���_�̍��W��F���v�Z���邱�ƁB

```cpp
	// Vertex shader ///////////////////////////////////////////////////////////////////////
	const char* vertexShaderSource = "#version 330 core\n"  //GLSL�ŏ����ꂽ�v�Z����
		"layout (location=0)in vec3 aPos;\n"
		"void main(){\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Shader�쐬
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Shader�ƃ\�[�X�R�[�h��Ή�
	glCompileShader(vertexShader); //�R���p�C��

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
```

* `vertexShaderSource`�FGLSL�ŏ����ꂽ�R�[�h�B�����ł�`location=0`�̃f�[�^�ɑ΂��ď������s�����Ƃ��Ă���B
* `glCompileShader()�FvertexShader`���R���p�C��

## Fragment shader
�X�N���[���̊e��f�ɂ��Ď��s�����B���ꂼ��̃s�N�Z���Ƃ������Ȃ�̐F
�œh�邩���v�Z����B

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

* `out`�F���̕ϐ��͏o�͂ł��邱�Ƃ��w�肷��
* `FragColor`�F�o�͂ƂȂ�ϐ���

## Linking Vertex Attributes
���܂̏�Ԃ�
* GPU�ɒ��_�f�[�^�𑗐Ms
* Vertex shader���R���p�C��
* Fragment shader���R���p�C��

���̂܂܂ł͉����\������Ȃ��B�܂��V�F�[�_�[��VBO�͂Ȃɂ��֘A���Ă��Ȃ��B
hader��VBO���֘A�t����K�v������B

```cpp
// Layout (loction=0)�Ƃ��ăV�F�[�_�[�ɓn��
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //Layout�̕ϐ���VBO��R�Â�
glEnableVertexAttribArray(0); //VBO�̒��g���V�F�[�_�[�ɑ���
```


�����A����������VBO������A����ɑ΂��ăV�F�[�_�[�ŏ����������Ƃ��A�����
`glVertexAttribPointer`��VBO��shader���w�肷��K�v���o�Ă���B

```cpp

// ---------- �ʒu����VBO ----------
unsigned int VBO_position;
glGenBuffers(1, &VBO_position);
glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

// layout (location = 0) �Ƃ��ăV�F�[�_�ɓn��
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// ---------- �F����VBO ----------
unsigned int VBO_color;
glGenBuffers(1, &VBO_color);
glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

// layout (location = 1) �Ƃ��ăV�F�[�_�ɓn��
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(1);
```
�܂�`glVertexAttribPointer()`�̐ݒ�͖��t���[�������邽�߁A���񃋁[�v��
�ݒ肷��K�v������B

```cpp
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, VBO); //���ݎg��VBO���w��
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //Layout�̕ϐ���VBO��R�Â�
		glEnableVertexAttribArray(0); //VBO�̒��g���V�F�[�_�[�ɑ���
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();  //�C�x���g�������callback���Ă�
	}
```

<details>

```cpp
// glad.h �͍ŏ��ɁI
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



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


	//vertex
	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
	};



	// VBO (Vertex Buffer Object) /////////////////////////////////////////////////////////////////////////////////
	unsigned int VBO;
	glGenBuffers(1, &VBO); //VBO���쐬
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //���_�̈ʒu����VBO���o�C���h;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //vertices��GPU�ɃA�b�v���[�h


	// ���_�����̐ݒ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //Layout�̕ϐ���VBO��R�Â�
	glEnableVertexAttribArray(0); //VBO�̒��g���V�F�[�_�[�ɑ���

	// Vertex shader ///////////////////////////////////////////////////////////////////////
	const char* vertexShaderSource = "#version 330 core\n"  //GLSL�ŏ����ꂽ�v�Z����
		"layout (location=0)in vec3 aPos;\n"
		"void main(){\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Shader�쐬
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Shader�ƃ\�[�X�R�[�h��Ή�
	glCompileShader(vertexShader); //�R���p�C��

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
	shaderProgram = glCreateProgram(); // shader program�쐬
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); // vertex shader��fragment shader���A�^�b�` (���X�^���C�Y�͏���ɂ���Ă����) 
	glLinkProgram(shaderProgram); // vertex shader�̏o�͂�fragment shader�̓��͂��������Ȃ����Ă��Ȃ��ƃG���[

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

		glBindBuffer(GL_ARRAY_BUFFER, VBO); //���ݎg��VBO���w��
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //Layout�̕ϐ���VBO��R�Â�
		glEnableVertexAttribArray(0); //VBO�̒��g���V�F�[�_�[�ɑ���
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();  //�C�x���g�������callback���Ă�
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
**VAO** (Vertex Array Object) �̓��[�v�̂��тɂŒ��_�ƃV�F�[�_�[�����т���K�v���Ȃ��Ȃ�B
��̓I�ɂ͈ȉ��̏���ێ�����
* `glEnableVertexAttribArray`�̌Ăяo�����
* `glVertexAttibPointer`�ɂ���Đݒ肳�ꂽ���_����
* `glVertexAttribPointer`��ʂ��Ē��_�����Ɋ֘A�t����ꂽVBO

<details>

```cpp
// glad.h �͍ŏ��ɁI
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



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

	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO�̃o�C���h������
	glBindVertexArray(0); //VAO�̃o�C���h������

	// Vertex shader ///////////////////////////////////////////////////////////////////////
	const char* vertexShaderSource = "#version 330 core\n"  //GLSL�ŏ����ꂽ�v�Z����
		"layout (location=0)in vec3 aPos;\n"
		"void main(){\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Shader�쐬
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Shader�ƃ\�[�X�R�[�h��Ή�
	glCompileShader(vertexShader); //�R���p�C��

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
	shaderProgram = glCreateProgram(); // shader program�쐬
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); // vertex shader��fragment shader���A�^�b�` (���X�^���C�Y�͏���ɂ���Ă����) 
	glLinkProgram(shaderProgram); // vertex shader�̏o�͂�fragment shader�̓��͂��������Ȃ����Ă��Ȃ��ƃG���[

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
		glfwPollEvents();  //�C�x���g�������callback���Ă�
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

### �l�p�`�̕`��


<details>

```cpp
// glad.h �͍ŏ��ɁI
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



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

	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO�̃o�C���h������
	glBindVertexArray(0); //VAO�̃o�C���h������

	// Vertex shader ///////////////////////////////////////////////////////////////////////
	const char* vertexShaderSource = "#version 330 core\n"  //GLSL�ŏ����ꂽ�v�Z����
		"layout (location=0)in vec3 aPos;\n"
		"void main(){\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Shader�쐬
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Shader�ƃ\�[�X�R�[�h��Ή�
	glCompileShader(vertexShader); //�R���p�C��

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
	shaderProgram = glCreateProgram(); // shader program�쐬
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); // vertex shader��fragment shader���A�^�b�` (���X�^���C�Y�͏���ɂ���Ă����) 
	glLinkProgram(shaderProgram); // vertex shader�̏o�͂�fragment shader�̓��͂��������Ȃ����Ă��Ȃ��ƃG���[

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
		glfwPollEvents();  //�C�x���g�������callback���Ă�
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

