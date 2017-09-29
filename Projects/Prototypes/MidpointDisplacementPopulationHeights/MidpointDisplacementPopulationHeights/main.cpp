// #define GLFW_STATIC
#include "Camera.h"
#include <GLFW\glfw3.h>
#include <stdlib.h>
#include <math.h>

/*
References:
1) https://r3dux.org/2012/12/a-c-camera-class-for-simple-opengl-fps-controls/
*/

Camera *cam;
GLFWwindow* window;

GLint windowWidth = 800;              // Width of our window
GLint windowHeight = 600;              // Heightof our window

GLint midWindowX = windowWidth / 2; // Middle of the window horizontally
GLint midWindowY = windowHeight / 2; // Middle of the window vertically

GLfloat fieldOfView = 45.0f;            // Define our field of view (i.e. how quickly foreshortening occurs)
GLfloat near = 2.0f;             // The near (Z Axis) point of our viewing frustum (default 2.0f)
GLfloat far = 1500.0f;          // The far  (Z Axis) point of our viewing frustum (default 1500.0f)

void InitGL()
{
	// GLFW settings

	// Disable Vsync
	glfwSwapInterval(0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// The following code is a fancy bit of math that is eqivilant to calling:
	// gluPerspective(fieldOfView / 2.0f, width / height, near, far);
	// We do it this way simply to avoid requiring glu.h
	GLfloat aspectRatio = (windowWidth > windowHeight) ? float(windowWidth) / float(windowHeight) : float(windowHeight) / float(windowWidth);
	GLfloat fH = tan(float(fieldOfView / 360.0f * 3.14159f)) * near;
	GLfloat fW = fH * aspectRatio;
	glFrustum(-fW, fW, -fH, fH, near, far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//OpenGL Settings
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);              // Set out clear colour to black, full alpha
	glEnable(GL_DEPTH_TEST);                           // Enable the depth buffer
	glClearDepth(1.0f);                                // Clear the entire depth of the depth buffer
	glDepthFunc(GL_LEQUAL);		                       // Set our depth function to overwrite if new value less than or equal to current value
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Ask for nicest perspective correction
	glLineWidth(2.0f);			                       // Set a 'chunky' line width

}

void KeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case 'W':
			cam->holdingForward = true;
			break;
		case 'A':
			cam->holdingLeftStrafe = true;
			break;
		case 'S':
			cam->holdingBackward = true;
			break;
		case 'D':
			cam->HoldingRightStrafe = true;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (key)
		{
		case 'W':
			cam->holdingForward = false;
			break;
		case 'A':
			cam->holdingLeftStrafe = false;
			break;
		case 'S':
			cam->holdingBackward = false;
			break;
		case 'D':
			cam->HoldingRightStrafe = false;
			break;
		default:
			break;
		}
	}
}

void HandleMouseMove(int mouseX, int mouseY)
{
	cam->handleMouseMove(mouseX, mouseY);
}

void DrawGround(int z)
{

}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Move the camera to our location in space
	glRotatef(cam->getXRot(), 1.0f, 0.0f, 0.0f); // Rotate our camera on the x-axis (looking up and down)
	glRotatef(cam->getYRot(), 0.0f, 1.0f, 0.0f); // Rotate our camera on the  y-axis (looking left and right)

												 // Translate the ModelView matrix to the position of our camera - everything should now be drawn relative
												 // to this position!
	glTranslatef(-cam->getXPos(), -cam->getYPos(), -cam->getZPos());

	DrawGround(-100.0f); // Draw lower ground grid
	DrawGround(100.0f);  // Draw upper ground grid

	// ----- Stop Drawing Stuff! ------
	glfwSwapBuffers(window);
}

int main(int ragc, char* argv[])
{
	// Frame counter and window settings variables
	int redBits = 8, greenBits = 8, blueBits = 8;
	int alphaBits = 8, depthBits = 24, stencilBits = 0;

	bool running = true;

	// Init GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	// Window
	window = glfwCreateWindow(640, 480, "Title", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Init GL
	InitGL();

	// Camera
	cam = new Camera(windowWidth, windowHeight, window);

	// Mouse
	glfwSetCursorPos(window, midWindowX, midWindowY);

	glfwSetKeyCallback(window, KeyPress);

	// glfwSetCursorPosCallback(HandleMouseMove);

	// The deltaTime variable keeps track of how much time has elapsed between one frame and the next.
	// This allows us to perform framerate independent movement i.e. the camera will move at the same
	// overall speed regardless of whether the app's running at (for example) 6fps, 60fps or 600fps!
	double deltaTime = 0.0;

	while (running)
	{
		// Calculate our camera movement
		cam->Move(deltaTime);

		// Draw our scene
		Display();

	}

	// Clean up GLFW and exit
	glfwTerminate();

	delete cam; // Delete our pointer to the camera object

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}