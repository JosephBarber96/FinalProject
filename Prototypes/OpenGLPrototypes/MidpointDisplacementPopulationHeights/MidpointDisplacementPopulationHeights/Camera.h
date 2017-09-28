#pragma once

/*
Credits:
https://r3dux.org/2012/12/a-c-camera-class-for-simple-opengl-fps-controls/
*/

#include <math.h>
#include <GLFW\glfw3.h>
#include "CVec3.hpp"

class Camera
{
public:
	Camera(float windowWidth, float windowHeight, GLFWwindow* argWindow);
	~Camera();

	static const double TO_RADS;

	bool holdingForward;
	bool holdingBackward;
	bool holdingLeftStrafe;
	bool HoldingRightStrafe;

	GLFWwindow* window;

	void handleMouseMove(int mousX, int mouseY);

	const double toRads(const double &angleInDegrees) const;

	void Move(double deltaTime);

	// Inline methods
	float getPitchSensitity() { return pitchSensitivity; }
	void SetPitchSensitivity(float i) { pitchSensitivity = i; }
	float GetYawSensitivity() { return yawSensitivity; }
	void SetYawSensitivity(float i) { yawSensitivity = i; }

	CVec3<double> GetPosition() const { return position; }
	double getXPos() const { return position.getX(); }
	double getYPos() const { return position.getY(); }
	double getZPos() const { return position.getZ(); }

	CVec3<double> getRotation() const { return rotation; }
	double getXRot() const { return rotation.getX(); }
	double getYRot() const { return rotation.getY(); }
	double getZRot() const { return rotation.getZ(); }

protected:
	CVec3<double> position;
	CVec3<double> rotation;
	CVec3<double> speed;

	double movementSpeedFactor;
	double pitchSensitivity;
	double yawSensitivity;

	int windowWidth;
	int windowHeight;
	int windowMidX;
	int windowMidY;

	void InitCamera();
};