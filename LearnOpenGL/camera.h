#pragma once
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

class Camera {
public:
	Camera(glm::vec3 position,glm::vec3 front,glm::vec3 up);
	Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch, float roll);
	~Camera();

	void updatePosition(Camera_Movement direction, float deltaTime);
	void updateDirection(double xpos, double ypos);
	void updateFov(double yoffset);

	glm::vec3 getPosition() const { return mPosition; }
	glm::vec3 getFront() const { return mFront; }
	glm::vec3 getUp() const { return mUp; }
	float getFov()const { return mFov; }

	

private:
	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;

	float mYaw;
	float mPitch;
	float mRoll;

	float mSpeed; //カメラの移動の速さ

	float mSensitivity;

	bool isFirstMouse; //マウスが初めて入ってきたかどうか

	float lastX; //前回のマウスのX座標
	float lastY; //前回のマウスのY座標

	float mFov; //視野角


};