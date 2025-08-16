# Camera
## Camera/View space
カメラは4つのベクトルで構成される。
* Position
* Direction
* Right
* Up

![](https://learnopengl.com/img/getting-started/camera_axes.pnghttps://learnopengl.com/img/getting-started/camera_axes.png

## Position
カメラの位置は、ワールド空間におけるカメラの位置を表す3次元ベクトルである。
```cpp
glm::vec3 cameraPos=glm::vec3(0.0f,0.0f,3.0f);
```

### Direction
カメラの方向は、カメラが向いている方向を表す3次元ベクトルである。もし、カメラが (0,0,0) を向いている場合、
```cpp
glm::vec3 cameraTarget=glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection=glm::normalize(cameraPos-cameraTarget);
```
となる。

### Right axis
Right axisはカメラの右方向を表す3次元ベクトルである。このベクトルは向いている方向と、ワーフ度座標の上方向
を外積して求めることが出来る。
```cpp
glm::vec3 up=glm::vec3(0.0f,1.0f,0.0f);
glm::vec3 cameraRight=glm::normalize(glm::cross(cameraDirection,up));
```

### Up axis
Up axisはカメラの右方向とカメラの向いている方向の外積で求めることが出来る。
```cpp
glm::vec3 cameraUp=glm::cross(cameraDirection,cameraRight);
```
Direction、Up、Rightの3つのベクトルはカメラに対する基底ベクトルである。

## LookAt Matrix
では、実際にカメラを実装してみる。カメラを実装するということは、World spaceから
View matrixを用いてView spaceに変換することを意味する。この変換はLookAt matrixを用いて行う。これは
glmで搭載されている
```cpp
glm::mat4 view;
view=glm::lookAt(glm::vec3(0.0f,0.0f,3.0f), // カメラの位置
				 glm::vec3(0.0f,0.0f,0.0f), // カメラの向いている方向
				 glm::vec3(0.0f,1.0f,0.0f)); // 上方向
```

worldの原点(0,0,0)を向いた、回転するカメラを実装する
```cpp
const float radius = 10.0f;
float camX = sin(glfwGetTime()) * radius;
float camZ = cos(glfwGetTime()) * radius;
glm::mat4 view;
view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); 
```
Directionが正面の(0,0,-1)固定で、wasdキーで移動するカメラを実装する。そのさい、コンピュータの性能によって、
移動速さが異ならないように、速さをdeltaTimeで調整する。
```cpp
void processInput(GLFWwindow* window) {
	// 移動
	const float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}
```
## Look around
カメラの回転はオイラー角で表すことが出来る
![](https://learnopengl.com/img/getting-started/camera_pitch_yaw_roll.png)
rollは一旦無視して、yawとpitchを用いてカメラの向きを決める
```cpp
direction.x=cos(glm::radiasn(yaw))*cos(glm::radians(pitch));
diraction.y=sin(glm::radians(pitch));
direction.z=sin(glm::radians(yaw))*cos(glm::radians(pitch));
```
初めは向きは-z方向であるため、
```cpp
yaw=-90.0f;
```
とする必要がある。

### Mouse input
マウスの入力を受け取るために、GLFWのコールバック関数を設定する。その前に、カーソルがウィンドウの
外に出ないように、ウィンドウの中心に固定する。FPSのようなイメージ。
```cpp
glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
```

カメラのマウス入力を扱うとき、カメラの方向ベクトルを完全に計算できるようにするまでにいくつかの手順
が必用
1. 前のフレームからのマウス移動量
2. そのオフセット値をカメラのよーとピッチに加える
3. ピッチ値に最小値、最大値の制約をかける
4. 方向ベクトルを計算する

まずコールバック関数でマウスの位置を取得する。
```cpp
glfwSetCursorPosCallback(window, mouse_callback);  
```
関数の中で、マウスの移動量を取得し、回転を計算する
```cpp
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}  
```

## Zoom
カメラのズームは、マウスをスクルールすることで行う。視野角を変更することで、ズームを実現する。
```cpp
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f; 
}
```


