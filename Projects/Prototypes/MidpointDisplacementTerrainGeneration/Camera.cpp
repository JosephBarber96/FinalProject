#include "Camera.h"
#include "Vector3.h"
#include <math.h>

Camera::Camera() {}
Camera::~Camera() {}

void Camera::MoveCamera(float speed)
{
	// Get the view vector
	Vector3 viewVector = *camView - *camPos;

	// Forward positive camera speed and backward negative -cameraspeed
	camPos->x = camPos->x + viewVector.x * speed;
	camPos->z = camPos->z + viewVector.z * speed;

	camView->x = camView->x + viewVector.x * speed;
	camView->z = camView->z + viewVector.z * speed;
}

void Camera::RotateView(float speed)
{
	// Get the view vector
	Vector3 viewVector = *camView - *camPos;

	camView->z = (float)(camPos->z + sin(speed) * viewVector.x + cos(speed) * viewVector.z);
	camView->x = (float)(camPos->x + cos(speed) * viewVector.x - sin(speed) * viewVector.z);
}

void Camera::PositionCamera
	(float posX, float posY, float posZ,
	float viewX, float viewY, float viewZ,
	float upX, float upY, float upZ)
{
	camPos = new Vector3(posX, posY, posZ);
	camView = new Vector3(viewX, viewY, viewZ);
	camUp = new Vector3(upX, upY, upZ);
}