#pragma once
class Vector3
{
public:
	Vector3();
	Vector3(float newX, float newY, float newZ);
	~Vector3();

	Vector3 operator+(Vector3 vec)
	{
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	}

	Vector3 operator-(Vector3 vec)
	{
		return Vector3(x - vec.x, y - vec.y, z - vec.z);
	}

	Vector3 operator*(float scale)
	{
		return Vector3(x * scale, y * scale, z * scale);
	}

	Vector3 operator/(float scale)
	{
		return Vector3(x / scale, y / scale, z / scale);
	}

	float x, y, z;

};

