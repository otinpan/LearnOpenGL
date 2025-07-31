# Shader

## `in` and `out`
`in`��`out`�̓V�F�[�_�[�قŃf�[�^���󂯓n�����߂̏C���q�B
��ɒ��_�V�F�[�_�[�A�t���O�����g�V�F�[�_�[�ԂŎg����B
```cpp
	float vertices2[] = {
		//position       // color 
		0.25f,0.0f,0.0f,  1.0f,0.0f,0.0f,
		-0.25f,0.0f,0.0f, 0.0f,1.0f,0.0f,
		0.0f,0.5f,0.0f,   0.0f,0.0f,1.0f
	};
```

�Ⴆ�΂��̂悤�ȎO�p�`�ɑ΂��āAshader���g���ꍇ�A
�܂�vertex shder (���_�V�F�[�_�[) �Œ��_�f�[�^���󂯎��K�v������B

`vertex.glsl`
```glsl
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;
```

`location`�Ƃ̓V�F�[�_�[���̕ϐ��ƁACPU���̃f�[�^��
���т��邽�߂̃C���f�b�N�X�ł���B
* `aPos`��location=0
* `aColor`��location=1

�Ɍ��т��B

�����āA�V�F�[�_�[����O���ɏo�͂������ꍇ��`out`�C���q���g��
```glsl
out vec3 ourColor;
```

`vertex.glsl`�S�̐}
```glsl
#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;
out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos.x+xOffset,aPos.y+yOffset,aPos.z, 1.0); // see how we directly give a vec3 to vec4's constructor
    ourColor = aColor;
}
```

`aPos`(�ʒu���W)�A`aColor`(���_�̐F)���󂯎��A`ourColor`�Ƃ��āA�O���ɏo�͂���B
���̊O���Ƃ����̂͊�{�I�ɂ�fragment shader (�t���O�����g�V�F�[�_�[) �ɓ�����B  
`fragment.glsl`
```glsl
#version 330 core
out vec4 FragColor;
in vec3 ourColor;

void main() {
    FragColor = vec4(ourColor, 1.0);
}
```

veertex shader����fragment shader��`ourColor`�Ƃ����F�̏���������
�f�[�^���n���ꂽ�B


## Uniform
`uniform`�̓V�F�[�_�[�Ƀf�[�^��n������1�̕��@�ł���B`uniform`���g����
�A�v���P�[�V������ (C++�Ȃ�) ����V�F�[�_�[�Ƀf�[�^��n�����Ƃ��o����B

`main.cpp`
```cpp
float timeValue = glfwGetTime();
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
int vertexColorLocation = glGetUniformLocation(mShader.getShaderProgram(), "changeColor");
mShader.use();
mShader.setBool("useChangeColor", true);
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // uniform������ahsder
mTriangle.draw()
```

`fragment.glsl`
```glsl
#version 330 core
out vec4 FragColor;
in vec3 ourColor;

uniform bool useChangeColor;
uniform vec4 changeColor;

void main() {
    if (useChangeColor)
        FragColor = changeColor;
    else
        FragColor = vec4(ourColor, 1.0);
}

```

* `int vertexColorLocation = glGetUniformLocation(mShader.getShaderProgram(), "changeColor");`
�ŃV�F�[�_�[����`changeColor`�Ƃ����ϐ���`uniform`�ɂ��V�F�[�_�[��
�C���f�b�N�X��T���B

* `glUniform4f(location, vec4)`���g���ƁA��قǒT�����C���f�b�N�X��4�����̃f�[�^��n�����Ƃ��o����
* `glUniform1b(location, bool)`���g���ƁA�u�[���l��n�����Ƃ��o����B


