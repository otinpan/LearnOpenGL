# Shader

## `in` and `out`
`in`と`out`はシェーダー館でデータを受け渡すための修飾子。
主に頂点シェーダー、フラグメントシェーダー間で使われる。
```cpp
	float vertices2[] = {
		//position       // color 
		0.25f,0.0f,0.0f,  1.0f,0.0f,0.0f,
		-0.25f,0.0f,0.0f, 0.0f,1.0f,0.0f,
		0.0f,0.5f,0.0f,   0.0f,0.0f,1.0f
	};
```

例えばこのような三角形に対して、shaderを使い場合、
まずvertex shder (頂点シェーダー) で頂点データを受け取る必要がある。

`vertex.glsl`
```glsl
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;
```

`location`とはシェーダー内の変数と、CPU側のデータを
結びつけるためのインデックスである。
* `aPos`はlocation=0
* `aColor`はlocation=1

に結びつく。

そして、シェーダーから外部に出力したい場合は`out`修飾子を使う
```glsl
out vec3 ourColor;
```

`vertex.glsl`全体図
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

`aPos`(位置座標)、`aColor`(頂点の色)を受け取り、`ourColor`として、外部に出力する。
この外部というのは基本的にはfragment shader (フラグメントシェーダー) に当たる。  
`fragment.glsl`
```glsl
#version 330 core
out vec4 FragColor;
in vec3 ourColor;

void main() {
    FragColor = vec4(ourColor, 1.0);
}
```

veertex shaderからfragment shaderに`ourColor`という色の情報を持った
データが渡された。


## Uniform
`uniform`はシェーダーにデータを渡すもう1つの方法である。`uniform`を使うと
アプリケーション側 (C++など) からシェーダーにデータを渡すことが出来る。

`main.cpp`
```cpp
float timeValue = glfwGetTime();
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
int vertexColorLocation = glGetUniformLocation(mShader.getShaderProgram(), "changeColor");
mShader.use();
mShader.setBool("useChangeColor", true);
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // uniformがあるahsder
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
でシェーダーから`changeColor`という変数を`uniform`にもつシェーダーの
インデックスを探す。

* `glUniform4f(location, vec4)`を使うと、先ほど探したインデックスに4次元のデータを渡すことが出来る
* `glUniform1b(location, bool)`を使うと、ブール値を渡すことが出来る。


