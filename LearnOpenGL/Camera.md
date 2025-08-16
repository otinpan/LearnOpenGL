# Camera
## Camera/View space
�J������4�̃x�N�g���ō\�������B
* Position
* Direction
* Right
* Up

![](https://learnopengl.com/img/getting-started/camera_axes.pnghttps://learnopengl.com/img/getting-started/camera_axes.png

## Position
�J�����̈ʒu�́A���[���h��Ԃɂ�����J�����̈ʒu��\��3�����x�N�g���ł���B
```cpp
glm::vec3 cameraPos=glm::vec3(0.0f,0.0f,3.0f);
```

### Direction
�J�����̕����́A�J�����������Ă��������\��3�����x�N�g���ł���B�����A�J������ (0,0,0) �������Ă���ꍇ�A
```cpp
glm::vec3 cameraTarget=glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection=glm::normalize(cameraPos-cameraTarget);
```
�ƂȂ�B

### Right axis
Right axis�̓J�����̉E������\��3�����x�N�g���ł���B���̃x�N�g���͌����Ă�������ƁA���[�t�x���W�̏����
���O�ς��ċ��߂邱�Ƃ��o����B
```cpp
glm::vec3 up=glm::vec3(0.0f,1.0f,0.0f);
glm::vec3 cameraRight=glm::normalize(glm::cross(cameraDirection,up));
```

### Up axis
Up axis�̓J�����̉E�����ƃJ�����̌����Ă�������̊O�ςŋ��߂邱�Ƃ��o����B
```cpp
glm::vec3 cameraUp=glm::cross(cameraDirection,cameraRight);
```
Direction�AUp�ARight��3�̃x�N�g���̓J�����ɑ΂�����x�N�g���ł���B

## LookAt Matrix
�ł́A���ۂɃJ�������������Ă݂�B�J��������������Ƃ������Ƃ́AWorld space����
View matrix��p����View space�ɕϊ����邱�Ƃ��Ӗ�����B���̕ϊ���LookAt matrix��p���čs���B�����
glm�œ��ڂ���Ă���
```cpp
glm::mat4 view;
view=glm::lookAt(glm::vec3(0.0f,0.0f,3.0f), // �J�����̈ʒu
				 glm::vec3(0.0f,0.0f,0.0f), // �J�����̌����Ă������
				 glm::vec3(0.0f,1.0f,0.0f)); // �����
```

world�̌��_(0,0,0)���������A��]����J��������������
```cpp
const float radius = 10.0f;
float camX = sin(glfwGetTime()) * radius;
float camZ = cos(glfwGetTime()) * radius;
glm::mat4 view;
view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)); 
```
Direction�����ʂ�(0,0,-1)�Œ�ŁAwasd�L�[�ňړ�����J��������������B���̂����A�R���s���[�^�̐��\�ɂ���āA
�ړ��������قȂ�Ȃ��悤�ɁA������deltaTime�Œ�������B
```cpp
void processInput(GLFWwindow* window) {
	// �ړ�
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
�J�����̉�]�̓I�C���[�p�ŕ\�����Ƃ��o����
![](https://learnopengl.com/img/getting-started/camera_pitch_yaw_roll.png)
roll�͈�U�������āAyaw��pitch��p���ăJ�����̌��������߂�
```cpp
direction.x=cos(glm::radiasn(yaw))*cos(glm::radians(pitch));
diraction.y=sin(glm::radians(pitch));
direction.z=sin(glm::radians(yaw))*cos(glm::radians(pitch));
```
���߂͌�����-z�����ł��邽�߁A
```cpp
yaw=-90.0f;
```
�Ƃ���K�v������B

### Mouse input
�}�E�X�̓��͂��󂯎�邽�߂ɁAGLFW�̃R�[���o�b�N�֐���ݒ肷��B���̑O�ɁA�J�[�\�����E�B���h�E��
�O�ɏo�Ȃ��悤�ɁA�E�B���h�E�̒��S�ɌŒ肷��BFPS�̂悤�ȃC���[�W�B
```cpp
glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
```

�J�����̃}�E�X���͂������Ƃ��A�J�����̕����x�N�g�������S�Ɍv�Z�ł���悤�ɂ���܂łɂ������̎菇
���K�p
1. �O�̃t���[������̃}�E�X�ړ���
2. ���̃I�t�Z�b�g�l���J�����̂�[�ƃs�b�`�ɉ�����
3. �s�b�`�l�ɍŏ��l�A�ő�l�̐����������
4. �����x�N�g�����v�Z����

�܂��R�[���o�b�N�֐��Ń}�E�X�̈ʒu���擾����B
```cpp
glfwSetCursorPosCallback(window, mouse_callback);  
```
�֐��̒��ŁA�}�E�X�̈ړ��ʂ��擾���A��]���v�Z����
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
�J�����̃Y�[���́A�}�E�X���X�N���[�����邱�Ƃōs���B����p��ύX���邱�ƂŁA�Y�[������������B
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


