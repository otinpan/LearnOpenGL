# Basic Lighting
�I�u�W�F�N�g�̐F�����肷�邽�߂ɂ͎��3�̗v�f���K�p�B
1. Ambient lighting
2. Diffuse lighting
3. Specular lighting

![](https://learnopengl.com/img/lighting/basic_lighting_phong.png)

## Ambient Lighting
�I�u�W�F�N�g�͂���1�̌��������łȂ��A�������������̌����󂯂�B�����A�S�Ă̌��������
�����v�Z����͔̂�����ł���BAmbient lighting�̓I�u�W�F�N�g�ɌW���������邱�ƂŁA�����\������B

```glsl
void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
} 
```
![](https://learnopengl.com/img/lighting/ambient_lighting.png)

## Diffuse lighting
����̓I�u�W�F�N�g�ƌ����̊p�x�Ɉˑ�����悤�ɁA���邳��ω�������B���ƃI�u�W�F�N�g�̕\�ʂ�
�����Ȃ�΁A�I�u�W�F�N�g�͖��邭�f��͂��ł���B����͌�������̃t���O�����g�̃x�N�g���ƁA
�I�u�W�F�N�g�̖@���x�N�g���̓��ς��v�Z���邱�ƂŎ����ł���B

### Normal Vector
�����ł�normal vector��vertex�����悤�ɂ���
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

�����āA����normal���V�F�[�_�[�ɓn���B���ɁA��������t���O�����g�̃x�N�g�����v�Z����B�����
�����̒��_�̈ʒu�ƌ����̈ʒu�̍��ł���B  
vertex
```glsl
#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
	gl_Position =projection * view * model * vec4(aPos, 1.0);
	Normal = aNormal; 
	FragPos= vec3(model * vec4(aPos, 1.0));
}
```

fragment
```glsl
#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;


void main(){
	//ambient
	float ambient=0.1;
	//difuse
	vec3 norm=normalize(Normal);
	vec3 lightDir=normalize(lightPos-FragPos);

	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=diff*lightColor;

	vec3 result=(ambient+diffuse)*objectColor;
	FragColor=vec4(result,1.0f);
}
```

���̂܂܂ł́A�I�u�W�F�N�g���X�P�[�����O���ꂽ�Ƃ��@�����k��͕ω����Ă��܂��B
![](https://learnopengl.com/img/lighting/basic_lighting_normal_transformation.png)

���f���s��Œ��_�����[���h���W�ɕϊ�����ۂɁA�@���x�N�g�����ω�����\��������B�����
���f���s��̋t�]�u�s���@���x�N�g���ɓK�p���邱�Ƃŉ����ł���B
```glsl
Normal = mat3(transpose(inverse(model))) * aNormal;  
```
�t�s��̌v�Z�͏d�������߁A�e���_�ōs���̂ł͂Ȃ����߂�CPU��Ōv�Z���邱�Ƃ����������B

## Specular lighting
Specular lighting�͌����̔��˂�\������B����͌�������̃t���O�����g�̃x�N�g���ƁA�J��������
�̃t���O�����g�̃x�N�g�����瓾�邱�Ƃ��o����B
```glsl
	//specular
	float specularStrength=0.5;
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),256);
	vec3 specular=specularStrength * spec * lightColor;
```

������256�Ƃ�shininess�ƌĂ΂����̂ŁA���̔��˂̋�����\���B�l���傫���قǁA���̔��˂͉s���Ȃ�B
