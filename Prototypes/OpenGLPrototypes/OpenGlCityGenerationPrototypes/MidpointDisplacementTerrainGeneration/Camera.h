#pragma once
class Vector3;
class Camera
{
public:
	Camera();
	~Camera();

	Vector3* camPos;
	Vector3* camView;
	Vector3* camUp;

	void MoveCamera(float speed);
	void RotateView(float speed);
	void PositionCamera
		(float posX, float posY, float posZ,
		float viewX, float viewY, float viewZ,
		float upX, float upY, float upZ);
};

