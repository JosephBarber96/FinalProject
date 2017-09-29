/*
Credits to:
https://r3dux.org/2012/12/a-c-camera-class-for-simple-opengl-fps-controls/
*/

#include "Camera.h"

// The value of 1 degree in radians
const double Camera::TO_RADS = 3.141592654 / 180.0;

Camera::Camera(float argWindowWidth, float argWindowHeight, GLFWwindow* argWindow)
{
	InitCamera();

	windowHeight = argWindowHeight;
	windowWidth = argWindowWidth;
	window = argWindow;

	windowMidX = windowWidth / 2.0f;
	windowMidY = windowHeight / 2.0f;


	glfwSetCursorPos(argWindow, windowMidX, windowMidY);
}

Camera::~Camera() {}

void Camera::InitCamera()
{
	// Set rotation, pos and speed to zero
	position.zero();
	rotation.zero();
	speed.zero();

	// Speed, turning
	movementSpeedFactor = 100.0;
	pitchSensitivity = 0.2;
	yawSensitivity = 0.2;

	holdingForward = false;
	holdingBackward = false;
	holdingLeftStrafe = false;
	HoldingRightStrafe = false;
}

const double Camera::toRads(const double &theAngleInDegrees) const
{
	return theAngleInDegrees * TO_RADS;
}

void Camera::handleMouseMove(int mouseX, int mouseY)
{
	// Calculate our horizontal and vertical mouse movement from middle of the window
	double horizMovement = (mouseX - windowMidX + 1) * yawSensitivity;
	double vertMovement = (mouseY - windowMidY) * pitchSensitivity;

	std::cout << "Mid window values: " << windowMidX << "\t" << windowMidY << std::endl;
	std::cout << "Mouse values     : " << mouseX << "\t" << mouseY << std::endl;
	std::cout << horizMovement << "\t" << vertMovement << std::endl << std::endl;

	// Apply the mouse movement to our rotation vector. The vertical (look up and down)
	// movement is applied on the X axis, and the horizontal (look left and right)
	// movement is applied on the Y Axis
	rotation.addX(vertMovement);
	rotation.addY(horizMovement);

	// Limit loking up to vertically up
	if (rotation.getX() < -90)
	{
		rotation.setX(-90);
	}

	// Limit looking down to vertically down
	if (rotation.getX() > 90)
	{
		rotation.setX(90);
	}

	// If you prefer to keep the angles in the range -180 to +180 use this code
	// and comment out the 0 to 360 code below.
	//
	// Looking left and right. Keep the angles in the range -180.0f (anticlockwise turn looking behind) to 180.0f (clockwise turn looking behind)
	/*if (yRot < -180.0f)
	{
	yRot += 360.0f;
	}

	if (yRot > 180.0f)
	{
	yRot -= 360.0f;
	}*/

	// Looking left and right - keep angles in the range 0.0 to 360.0
	// 0 degrees is looking directly down the negative Z axis "North", 90 degrees is "East", 180 degrees is "South", 270 degrees is "West"
	// We can also do this so that our 360 degrees goes -180 through +180 and it works the same, but it's probably best to keep our
	// range to 0 through 360 instead of -180 through +180.
	if (rotation.getY() < 0)
	{
		rotation.addY(360);
	}
	if (rotation.getY() > 360)
	{
		rotation.addY(-360);
	}

	// Reset the mouse position to the centre of the window each frame
	glfwSetCursorPos(window, windowMidX, windowMidY);
}

// Function to calculate which direction we need to move the camera and by what amount
void Camera::Move(double deltaTime)
{
	// Vector to break up our movement into components along the X, Y and Z axis
	CVec3<double> movement;

	// Get the sine and cosine of our X and Y axis rotation
	double sinXRot = sin(toRads(rotation.getX()));
	double cosXRot = cos(toRads(rotation.getX()));

	double sinYRot = sin(toRads(rotation.getY()));
	double cosYRot = cos(toRads(rotation.getY()));

	double pitchLimitFactor = cosXRot; // This cancels out moving on the Z axis when we're looking up or down

	if (holdingForward)
	{
		movement.addX(sinYRot * pitchLimitFactor);
		movement.addY(-sinXRot);
		movement.addZ(-cosYRot * pitchLimitFactor);
	}

	if (holdingBackward)
	{
		movement.addX(-sinYRot * pitchLimitFactor);
		movement.addY(sinXRot);
		movement.addZ(cosYRot * pitchLimitFactor);
	}

	if (holdingLeftStrafe)
	{
		movement.addX(-cosYRot);
		movement.addZ(-sinYRot);
	}

	if (HoldingRightStrafe)
	{
		movement.addX(cosYRot);
		movement.addZ(sinYRot);
	}

	// Normalise our movement vector
	movement.normalise();

	// Calculate our value to keep the movement the same speed regardless of the framerate...
	double framerateIndependentFactor = movementSpeedFactor * deltaTime;

	// .. and then apply it to our movement vector.
	movement *= framerateIndependentFactor;

	// Finally, apply the movement to our position
	position += movement;
}