# Basic Lighting
オブジェクトの色を決定するためには主に3つの要素が必用。
1. Ambient lighting
2. Diffuse lighting
3. Specular lighting

![](https://learnopengl.com/img/lighting/basic_lighting_phong.png)

## Ambient Lighting
オブジェクトはある1つの光源だけでなく、あらゆる光源からの光を受ける。ただ、全ての光源からの
光を計算するのは非効率である。Ambient lightingはオブジェクトに係数を書けることで、これを表現する。

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
これはオブジェクトと光源の角度に依存するように、明るさを変化させる。光とオブジェクトの表面が
垂直ならば、オブジェクトは明るく映るはずである。これは光源からのフラグメントのベクトルと、
オブジェクトの法線ベクトルの内積を計算することで実現できる。

### Normal Vector
ここではnormal vectorはvertexが持つようにする
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
```

そして、このnormalをシェーダーに渡す。次に、光源からフラグメントのベクトルを計算する。これは
ただの頂点の位置と光源の位置の差である。  
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

今のままでは、オブジェクトがスケーリングされたとき法線博徒るは変化してしまう。
![](https://learnopengl.com/img/lighting/basic_lighting_normal_transformation.png)

モデル行列で頂点をワールド座標に変換する際に、法線ベクトルが変化する可能性がある。これは
モデル行列の逆転置行列を法線ベクトルに適用することで解決できる。
```glsl
Normal = mat3(transpose(inverse(model))) * aNormal;  
```
逆行列の計算は重たいため、各頂点で行うのではなく初めにCPU上で計算することが推奨される。

## Specular lighting
Specular lightingは光源の反射を表現する。これは光源からのフラグメントのベクトルと、カメラから
のフラグメントのベクトルから得ることが出来る。
```glsl
	//specular
	float specularStrength=0.5;
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),256);
	vec3 specular=specularStrength * spec * lightColor;
```

ここで256とはshininessと呼ばれるもので、光の反射の強さを表す。値が大きいほど、光の反射は鋭くなる。
