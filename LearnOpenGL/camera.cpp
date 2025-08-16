#include"camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) 
	:mPosition(position)
	,mFront(front)
	,mUp(up)
{
	mYaw = -90.0f;
	mPitch = 0.0f;
	mRoll = 0.0f;
	mSensitivity = 0.1f;
	isFirstMouse = true;
	lastX = 800 / 2.0f; //スクリーンの中心
	lastY = 600 / 2.0f; //スクリーンの中心
	mFov = 45.0f;
	mSpeed = 2.5f;
}

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float yaw, float pitch, float roll) 
	:mPosition(position)
	, mFront(front)
	, mUp(up)
	, mYaw(yaw)
	, mPitch(pitch)
	, mRoll(roll)
{
	mSensitivity = 0.1f;
	isFirstMouse = true;
	lastX = 800 / 2.0f; //スクリーンの中心
	lastY = 600 / 2.0f; //スクリーンの中心
	mFov = 45.0f;
	mSpeed = 2.5f;
}

Camera::~Camera() {

}

void Camera::updatePosition(Camera_Movement direction, float deltaTime) {
	const float cameraSpeed = mSpeed * deltaTime;
	if (direction == FORWARD) {
		mPosition += cameraSpeed * mFront;
	}
	if (direction == BACKWARD) {
		mPosition -= cameraSpeed * mFront;
	}
	if (direction == LEFT) {
		mPosition -= glm::normalize(glm::cross(mFront, mUp)) * cameraSpeed;
	}
	if (direction == RIGHT) {
		mPosition += glm::normalize(glm::cross(mFront, mUp)) * cameraSpeed;
	}
}

void Camera::updateDirection(double xpos, double ypos) {
	if (isFirstMouse) {
		lastX = xpos;
		lastY = ypos;
		isFirstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= mSensitivity;
	yoffset *= mSensitivity;

	mYaw += xoffset;
	mPitch += yoffset;

	if (mPitch > 89.0f) {
		mPitch = 89.0f;
	}
	if (mPitch < -89.0f) {
		mPitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x=cos(glm::radians(mYaw))* cos(glm::radians(mPitch));
	direction.y = sin(glm::radians(mPitch));
	direction.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(direction);
}

void Camera::updateFov(double yoffset) {
	mFov -= (float)yoffset;
	if (mFov < 1.0f) {
		mFov = 1.0f;
	}
	if (mFov > 45.0f) {
		mFov = 45.0f;
	}
}